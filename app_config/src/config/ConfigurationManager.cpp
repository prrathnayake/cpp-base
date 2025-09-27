#include "ConfigurationManager.h"

#include <cstdlib>
#include <stdexcept>
#include <string>
#include <utility>

#include "utils/json/json.h"
#include "utils/log/singletonLogger.h"

namespace app_config
{
    namespace
    {
        std::string defaultResolver(const std::string &key)
        {
            const char *value = std::getenv(key.c_str());
            return value ? std::string{value} : std::string{};
        }

        void mergeObject(nlohmann::json &target, const nlohmann::json &patch)
        {
            for (auto it = patch.begin(); it != patch.end(); ++it)
            {
                const auto &key = it.key();
                const auto &value = it.value();

                if (value.is_object() && target.contains(key) && target[key].is_object())
                {
                    mergeObject(target[key], value);
                    continue;
                }

                target[key] = value;
            }
        }

        const nlohmann::json *findByPath(const nlohmann::json &root, const std::string &key)
        {
            const nlohmann::json *current = &root;
            std::size_t start = 0U;
            while (start < key.size() && current)
            {
                const auto end = key.find('.', start);
                const auto token = key.substr(start, end == std::string::npos ? key.size() - start : end - start);
                if (!current->is_object() || !current->contains(token))
                {
                    return nullptr;
                }

                current = &(*current)[token];
                if (end == std::string::npos)
                {
                    break;
                }
                start = end + 1U;
            }

            return current;
        }

        void resolveNode(nlohmann::json &node, const ConfigurationManager::EnvResolver &resolver)
        {
            if (node.is_object())
            {
                for (auto &item : node.items())
                {
                    resolveNode(item.value(), resolver);
                }
                return;
            }

            if (!node.is_string())
            {
                return;
            }

            std::string value = node.get<std::string>();
            std::size_t pos = 0U;
            bool substituted = false;
            while ((pos = value.find("${", pos)) != std::string::npos)
            {
                const std::size_t end = value.find('}', pos + 2);
                if (end == std::string::npos)
                {
                    break;
                }

                const auto key = value.substr(pos + 2, end - (pos + 2));
                const auto resolved = resolver ? resolver(key) : std::string{};
                value.replace(pos, end - pos + 1, resolved);
                substituted = true;
                pos += resolved.size();
            }

            if (substituted)
            {
                node = value;
            }
        }
    }

    ConfigurationManager::ConfigurationManager(std::string serviceName)
        : resolver_(defaultResolver), serviceName_(std::move(serviceName))
    {
        utils::SingletonLogger::instance().logMeta(
            utils::SingletonLogger::MessageCode::INFO,
            "ConfigurationManager initialised for service '" + serviceName_ + "'",
            __FILE__,
            __LINE__,
            __func__);
    }

    void ConfigurationManager::loadDefaults(const std::string &filePath)
    {
        try
        {
            auto defaults = utils::Json::readFromFile(filePath);
            std::lock_guard<std::mutex> lock(mutex_);
            defaults_ = std::move(defaults);
            recomputeMerged();
            utils::SingletonLogger::instance().logMeta(
                utils::SingletonLogger::MessageCode::INFO,
                "Loaded default configuration from " + filePath,
                __FILE__,
                __LINE__,
                __func__);
        }
        catch (const std::exception &ex)
        {
            utils::SingletonLogger::instance().logMeta(
                utils::SingletonLogger::MessageCode::ERROR,
                std::string{"Failed to load default configuration: "} + ex.what(),
                __FILE__,
                __LINE__,
                __func__);
            throw;
        }
    }

    void ConfigurationManager::loadEnvironment(const std::string &filePath)
    {
        try
        {
            auto env = utils::Json::readFromFile(filePath);
            resolveEnvironmentPlaceholders(env, resolver_);
            std::lock_guard<std::mutex> lock(mutex_);
            environment_ = std::move(env);
            recomputeMerged();
            utils::SingletonLogger::instance().logMeta(
                utils::SingletonLogger::MessageCode::INFO,
                "Loaded environment configuration from " + filePath,
                __FILE__,
                __LINE__,
                __func__);
        }
        catch (const std::exception &ex)
        {
            utils::SingletonLogger::instance().logMeta(
                utils::SingletonLogger::MessageCode::ERROR,
                std::string{"Failed to load environment configuration: "} + ex.what(),
                __FILE__,
                __LINE__,
                __func__);
            throw;
        }
    }

    void ConfigurationManager::applyRuntimeOverrides(const nlohmann::json &overrides)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        runtime_ = overrides;
        recomputeMerged();
        utils::SingletonLogger::instance().logMeta(
            utils::SingletonLogger::MessageCode::INFO,
            "Applied runtime overrides",
            __FILE__,
            __LINE__,
            __func__);
    }

    void ConfigurationManager::setEnvironmentResolver(EnvResolver resolver)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        resolver_ = std::move(resolver);
        utils::SingletonLogger::instance().logMeta(
            utils::SingletonLogger::MessageCode::INFO,
            "Updated environment resolver",
            __FILE__,
            __LINE__,
            __func__);
    }

    bool ConfigurationManager::has(const std::string &key) const
    {
        std::lock_guard<std::mutex> lock(mutex_);
        return findPointerLocked(key) != nullptr;
    }

    nlohmann::json ConfigurationManager::raw() const
    {
        std::lock_guard<std::mutex> lock(mutex_);
        return merged_;
    }

    void ConfigurationManager::mergeLayer(const nlohmann::json &layer, const std::string &layerName)
    {
        utils::SingletonLogger::instance().logMeta(
            utils::SingletonLogger::MessageCode::DEBUG,
            "Merging configuration layer: " + layerName,
            __FILE__,
            __LINE__,
            __func__);
        deepMerge(merged_, layer);
    }

    const nlohmann::json *ConfigurationManager::findPointerLocked(const std::string &key) const
    {
        return findByPath(merged_, key);
    }

    void ConfigurationManager::recomputeMerged()
    {
        merged_ = nlohmann::json::object();
        mergeLayer(defaults_, "defaults");
        mergeLayer(environment_, "environment");
        mergeLayer(runtime_, "runtime");
    }

    void ConfigurationManager::deepMerge(nlohmann::json &target, const nlohmann::json &patch)
    {
        if (!patch.is_object())
        {
            return;
        }

        mergeObject(target, patch);
    }

    void ConfigurationManager::resolveEnvironmentPlaceholders(nlohmann::json &node, const EnvResolver &resolver)
    {
        resolveNode(node, resolver);
    }
}
