#pragma once

#include <chrono>
#include <optional>
#include <string>
#include <unordered_map>

namespace app_cache
{
    class CacheClient
    {
    public:
        void connect(const std::string &name, const std::string &connectionUri);

        bool set(const std::string &key, const std::string &value, std::chrono::seconds ttl = std::chrono::seconds{0});
        std::optional<std::string> get(const std::string &key);
        bool del(const std::string &key);
        bool exists(const std::string &key);

        void hset(const std::string &key, const std::string &field, const std::string &value);
        std::optional<std::string> hget(const std::string &key, const std::string &field);
        std::unordered_map<std::string, std::string> hgetall(const std::string &key);

        void flush();

    private:
        struct CacheEntry
        {
            std::string value;
            std::chrono::steady_clock::time_point expiresAt;
            bool hasExpiry{false};
        };

        using HashMap = std::unordered_map<std::string, std::string>;

        void purgeExpired();

        std::unordered_map<std::string, CacheEntry> kvStore_;
        std::unordered_map<std::string, HashMap> hashStore_;
    };
}

#include "CacheClient.tcc"
