#pragma once

#include <chrono>
#include <optional>
#include <string>

#include <nlohmann/json.hpp>

namespace app_auth
{
    struct ValidationResult
    {
        bool success{false};
        std::string message;
    };

    class JwtManager
    {
    public:
        JwtManager(std::string secret, std::string issuer, std::string audience);

        std::string issueToken(const nlohmann::json &payload, std::chrono::seconds ttl) const;
        ValidationResult verifyToken(const std::string &token, nlohmann::json &payloadOut) const;

        void rotateSecret(const std::string &newSecret);

    private:
        static std::string base64UrlEncode(const std::string &data);
        static std::optional<std::string> base64UrlDecode(const std::string &data);
        std::string sign(const std::string &headerPayload) const;
        static bool constantTimeEquals(const std::string &lhs, const std::string &rhs);

        std::string secret_;
        std::string issuer_;
        std::string audience_;
    };
}

#include "JwtManager.tcc"
