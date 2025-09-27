#include "JwtManager.h"

#include <array>
#include <cstring>
#include <stdexcept>
#include <vector>

#include <openssl/buffer.h>
#include <openssl/crypto.h>
#include <openssl/evp.h>
#include <openssl/hmac.h>

#include "utils/log/singletonLogger.h"

namespace app_auth
{
    namespace
    {
        std::string toJsonString(const nlohmann::json &value)
        {
            return value.dump();
        }

        std::string buildHeader()
        {
            nlohmann::json header = {
                {"alg", "HS256"},
                {"typ", "JWT"}
            };
            return toJsonString(header);
        }

        std::vector<std::string> splitToken(const std::string &token)
        {
            std::vector<std::string> parts;
            std::size_t start = 0U;
            for (int i = 0; i < 2; ++i)
            {
                const auto pos = token.find('.', start);
                if (pos == std::string::npos)
                {
                    return {};
                }
                parts.emplace_back(token.substr(start, pos - start));
                start = pos + 1U;
            }
            parts.emplace_back(token.substr(start));
            return parts;
        }

        std::string formatTimestamp(std::chrono::system_clock::time_point tp)
        {
            const auto seconds = std::chrono::duration_cast<std::chrono::seconds>(tp.time_since_epoch());
            return std::to_string(seconds.count());
        }
    }

    JwtManager::JwtManager(std::string secret, std::string issuer, std::string audience)
        : secret_(std::move(secret)), issuer_(std::move(issuer)), audience_(std::move(audience))
    {
        utils::SingletonLogger::instance().logMeta(
            utils::SingletonLogger::MessageCode::INFO,
            "JwtManager initialised for audience '" + audience_ + "'",
            __FILE__,
            __LINE__,
            __func__);
    }

    std::string JwtManager::issueToken(const nlohmann::json &payload, std::chrono::seconds ttl) const
    {
        const auto header = base64UrlEncode(buildHeader());

        const auto now = std::chrono::system_clock::now();
        nlohmann::json body = payload;
        body["iss"] = issuer_;
        body["aud"] = audience_;
        body["iat"] = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();
        body["exp"] = std::chrono::duration_cast<std::chrono::seconds>((now + ttl).time_since_epoch()).count();

        const auto payloadEncoded = base64UrlEncode(toJsonString(body));
        const auto signingInput = header + '.' + payloadEncoded;
        const auto signature = sign(signingInput);

        utils::SingletonLogger::instance().logMeta(
            utils::SingletonLogger::MessageCode::INFO,
            "Issued JWT expiring at " + formatTimestamp(now + ttl),
            __FILE__,
            __LINE__,
            __func__);

        return signingInput + '.' + signature;
    }

    ValidationResult JwtManager::verifyToken(const std::string &token, nlohmann::json &payloadOut) const
    {
        const auto parts = splitToken(token);
        if (parts.size() != 3)
        {
            utils::SingletonLogger::instance().logMeta(
                utils::SingletonLogger::MessageCode::ERROR,
                "JWT verification failed: malformed token",
                __FILE__,
                __LINE__,
                __func__);
            return {false, "Token format invalid"};
        }

        const auto signingInput = parts[0] + '.' + parts[1];
        const auto expectedSignature = sign(signingInput);
        if (!constantTimeEquals(expectedSignature, parts[2]))
        {
            utils::SingletonLogger::instance().logMeta(
                utils::SingletonLogger::MessageCode::WARNING,
                "JWT verification failed: signature mismatch",
                __FILE__,
                __LINE__,
                __func__);
            return {false, "Signature mismatch"};
        }

        auto payloadRaw = base64UrlDecode(parts[1]);
        if (!payloadRaw)
        {
            utils::SingletonLogger::instance().logMeta(
                utils::SingletonLogger::MessageCode::ERROR,
                "JWT verification failed: payload could not be decoded",
                __FILE__,
                __LINE__,
                __func__);
            return {false, "Payload decoding failed"};
        }

        try
        {
            payloadOut = nlohmann::json::parse(*payloadRaw);
        }
        catch (const std::exception &ex)
        {
            utils::SingletonLogger::instance().logMeta(
                utils::SingletonLogger::MessageCode::ERROR,
                std::string{"JWT verification failed: invalid JSON payload: "} + ex.what(),
                __FILE__,
                __LINE__,
                __func__);
            return {false, "Payload JSON invalid"};
        }

        const auto nowSeconds = std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::system_clock::now().time_since_epoch()).count();

        if (payloadOut.contains("exp") && payloadOut["exp"].is_number())
        {
            if (payloadOut["exp"].get<std::int64_t>() < nowSeconds)
            {
                utils::SingletonLogger::instance().logMeta(
                    utils::SingletonLogger::MessageCode::WARNING,
                    "JWT verification failed: token expired",
                    __FILE__,
                    __LINE__,
                    __func__);
                return {false, "Token expired"};
            }
        }

        if (payloadOut.contains("iss") && payloadOut["iss"].get<std::string>() != issuer_)
        {
            utils::SingletonLogger::instance().logMeta(
                utils::SingletonLogger::MessageCode::WARNING,
                "JWT verification failed: issuer mismatch",
                __FILE__,
                __LINE__,
                __func__);
            return {false, "Issuer mismatch"};
        }

        if (payloadOut.contains("aud") && payloadOut["aud"].get<std::string>() != audience_)
        {
            utils::SingletonLogger::instance().logMeta(
                utils::SingletonLogger::MessageCode::WARNING,
                "JWT verification failed: audience mismatch",
                __FILE__,
                __LINE__,
                __func__);
            return {false, "Audience mismatch"};
        }

        utils::SingletonLogger::instance().logMeta(
            utils::SingletonLogger::MessageCode::INFO,
            "JWT verification succeeded",
            __FILE__,
            __LINE__,
            __func__);
        return {true, "OK"};
    }

    void JwtManager::rotateSecret(const std::string &newSecret)
    {
        secret_ = newSecret;
        utils::SingletonLogger::instance().logMeta(
            utils::SingletonLogger::MessageCode::INFO,
            "JWT signing secret rotated",
            __FILE__,
            __LINE__,
            __func__);
    }

    std::string JwtManager::base64UrlEncode(const std::string &data)
    {
        const auto encodedLength = 4 * ((data.size() + 2) / 3);
        std::string encoded(static_cast<std::size_t>(encodedLength), '\0');
        const auto result = EVP_EncodeBlock(reinterpret_cast<unsigned char *>(&encoded[0]),
                                            reinterpret_cast<const unsigned char *>(data.data()),
                                            static_cast<int>(data.size()));
        if (result < 0)
        {
            throw std::runtime_error("Failed to base64 encode data");
        }
        encoded.resize(static_cast<std::size_t>(result));

        for (char &ch : encoded)
        {
            if (ch == '+')
            {
                ch = '-';
            }
            else if (ch == '/')
            {
                ch = '_';
            }
        }

        while (!encoded.empty() && encoded.back() == '=')
        {
            encoded.pop_back();
        }

        return encoded;
    }

    std::optional<std::string> JwtManager::base64UrlDecode(const std::string &data)
    {
        std::string normalised = data;
        for (char &ch : normalised)
        {
            if (ch == '-')
            {
                ch = '+';
            }
            else if (ch == '_')
            {
                ch = '/';
            }
        }

        while (normalised.size() % 4 != 0)
        {
            normalised.push_back('=');
        }

        std::string decoded((normalised.size() / 4) * 3, '\0');
        const auto result = EVP_DecodeBlock(reinterpret_cast<unsigned char *>(&decoded[0]),
                                            reinterpret_cast<const unsigned char *>(normalised.data()),
                                            static_cast<int>(normalised.size()));
        if (result < 0)
        {
            return std::nullopt;
        }

        decoded.resize(static_cast<std::size_t>(result));
        return decoded;
    }

    std::string JwtManager::sign(const std::string &headerPayload) const
    {
        unsigned int length = 0U;
        std::array<unsigned char, EVP_MAX_MD_SIZE> buffer{};

        const unsigned char *key = reinterpret_cast<const unsigned char *>(secret_.data());
        const unsigned char *data = reinterpret_cast<const unsigned char *>(headerPayload.data());

        HMAC(EVP_sha256(), key, static_cast<int>(secret_.size()), data, headerPayload.size(), buffer.data(), &length);
        return base64UrlEncode(std::string(reinterpret_cast<char *>(buffer.data()), length));
    }

    bool JwtManager::constantTimeEquals(const std::string &lhs, const std::string &rhs)
    {
        if (lhs.size() != rhs.size())
        {
            return false;
        }

        return CRYPTO_memcmp(lhs.data(), rhs.data(), lhs.size()) == 0;
    }
}
