#pragma once

#include <functional>
#include <mutex>
#include <optional>
#include <string>
#include <unordered_map>

#include <nlohmann/json.hpp>

namespace app_config
{
    class ConfigurationManager
    {
    public:
        using EnvResolver = std::function<std::string(const std::string&)>;

        explicit ConfigurationManager(std::string serviceName = "default");

        void loadDefaults(const std::string &filePath);
        void loadEnvironment(const std::string &filePath);
        void applyRuntimeOverrides(const nlohmann::json &overrides);

        void setEnvironmentResolver(EnvResolver resolver);

        [[nodiscard]] bool has(const std::string &key) const;

        template <typename T>
        [[nodiscard]] T get(const std::string &key, const T &fallback) const
        {
            std::lock_guard<std::mutex> lock(mutex_);
            const nlohmann::json *value = findPointerLocked(key);
            if (!value || value->is_null())
            {
                return fallback;
            }

            try
            {
                return value->get<T>();
            }
            catch (const std::exception &)
            {
                return fallback;
            }
        }

        [[nodiscard]] nlohmann::json raw() const;

    private:
        void mergeLayer(const nlohmann::json &layer, const std::string &layerName);
        [[nodiscard]] const nlohmann::json *findPointerLocked(const std::string &key) const;
        void recomputeMerged();
        static void deepMerge(nlohmann::json &target, const nlohmann::json &patch);
        static void resolveEnvironmentPlaceholders(nlohmann::json &node, const EnvResolver &resolver);

        nlohmann::json defaults_;
        nlohmann::json environment_;
        nlohmann::json runtime_;
        nlohmann::json merged_;

        EnvResolver resolver_;
        std::string serviceName_;

        mutable std::mutex mutex_;
    };
}

#include "ConfigurationManager.tcc"
