#include "CacheClient.h"

#include <mutex>

#include "utils/log/singletonLogger.h"

namespace app_cache
{
    namespace
    {
        std::mutex &globalMutex()
        {
            static std::mutex mutex;
            return mutex;
        }
    }

    void CacheClient::connect(const std::string &name, const std::string &connectionUri)
    {
        std::lock_guard<std::mutex> lock(globalMutex());
        utils::SingletonLogger::instance().logMeta(
            utils::SingletonLogger::MessageCode::INFO,
            "Cache client '" + name + "' initialised with URI " + connectionUri,
            __FILE__,
            __LINE__,
            __func__);
    }

    bool CacheClient::set(const std::string &key, const std::string &value, std::chrono::seconds ttl)
    {
        std::lock_guard<std::mutex> lock(globalMutex());
        CacheEntry entry{value, std::chrono::steady_clock::now() + ttl, ttl.count() > 0};
        kvStore_[key] = entry;
        utils::SingletonLogger::instance().logMeta(
            utils::SingletonLogger::MessageCode::INFO,
            "Cache SET '" + key + "' (ttl=" + std::to_string(ttl.count()) + ")",
            __FILE__,
            __LINE__,
            __func__);
        return true;
    }

    std::optional<std::string> CacheClient::get(const std::string &key)
    {
        std::lock_guard<std::mutex> lock(globalMutex());
        purgeExpired();
        const auto it = kvStore_.find(key);
        if (it == kvStore_.end())
        {
            utils::SingletonLogger::instance().logMeta(
                utils::SingletonLogger::MessageCode::DEBUG,
                "Cache MISS '" + key + "'",
                __FILE__,
                __LINE__,
                __func__);
            return std::nullopt;
        }

        utils::SingletonLogger::instance().logMeta(
            utils::SingletonLogger::MessageCode::INFO,
            "Cache HIT '" + key + "'",
            __FILE__,
            __LINE__,
            __func__);
        return it->second.value;
    }

    bool CacheClient::del(const std::string &key)
    {
        std::lock_guard<std::mutex> lock(globalMutex());
        const auto erased = kvStore_.erase(key);
        hashStore_.erase(key);
        utils::SingletonLogger::instance().logMeta(
            erased ? utils::SingletonLogger::MessageCode::INFO : utils::SingletonLogger::MessageCode::WARNING,
            "Cache DEL '" + key + "' => " + (erased ? "removed" : "missing"),
            __FILE__,
            __LINE__,
            __func__);
        return erased > 0;
    }

    bool CacheClient::exists(const std::string &key)
    {
        std::lock_guard<std::mutex> lock(globalMutex());
        purgeExpired();
        const bool found = kvStore_.count(key) > 0;
        utils::SingletonLogger::instance().logMeta(
            utils::SingletonLogger::MessageCode::DEBUG,
            "Cache EXISTS '" + key + "' => " + (found ? "true" : "false"),
            __FILE__,
            __LINE__,
            __func__);
        return found;
    }

    void CacheClient::hset(const std::string &key, const std::string &field, const std::string &value)
    {
        std::lock_guard<std::mutex> lock(globalMutex());
        hashStore_[key][field] = value;
        utils::SingletonLogger::instance().logMeta(
            utils::SingletonLogger::MessageCode::INFO,
            "Cache HSET '" + key + "'::'" + field + "'",
            __FILE__,
            __LINE__,
            __func__);
    }

    std::optional<std::string> CacheClient::hget(const std::string &key, const std::string &field)
    {
        std::lock_guard<std::mutex> lock(globalMutex());
        const auto keyIt = hashStore_.find(key);
        if (keyIt == hashStore_.end())
        {
            return std::nullopt;
        }

        const auto fieldIt = keyIt->second.find(field);
        if (fieldIt == keyIt->second.end())
        {
            return std::nullopt;
        }

        return fieldIt->second;
    }

    std::unordered_map<std::string, std::string> CacheClient::hgetall(const std::string &key)
    {
        std::lock_guard<std::mutex> lock(globalMutex());
        const auto it = hashStore_.find(key);
        if (it == hashStore_.end())
        {
            return {};
        }
        return it->second;
    }

    void CacheClient::flush()
    {
        std::lock_guard<std::mutex> lock(globalMutex());
        kvStore_.clear();
        hashStore_.clear();
        utils::SingletonLogger::instance().logMeta(
            utils::SingletonLogger::MessageCode::INFO,
            "Cache FLUSH executed",
            __FILE__,
            __LINE__,
            __func__);
    }

    void CacheClient::purgeExpired()
    {
        const auto now = std::chrono::steady_clock::now();
        for (auto it = kvStore_.begin(); it != kvStore_.end();)
        {
            if (it->second.hasExpiry && it->second.expiresAt <= now)
            {
                utils::SingletonLogger::instance().logMeta(
                    utils::SingletonLogger::MessageCode::DEBUG,
                    "Cache expired '" + it->first + "'",
                    __FILE__,
                    __LINE__,
                    __func__);
                it = kvStore_.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }
}
