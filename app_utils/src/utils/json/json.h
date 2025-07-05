#pragma once
#ifndef UTILS_JSON_H
#define UTILS_JSON_H

#include <string>
#include <nlohmann/json.hpp>

namespace utils {

    class Json {
    public:
        static nlohmann::json readFromFile(const std::string &filePath);
        static void writeToFile(const std::string &filePath, const nlohmann::json &j);
        static nlohmann::json parse(const std::string &jsonStr);
        static std::string toString(const nlohmann::json &j);

        static bool isValidJson(const std::string &jsonStr);
        static bool hasKey(const nlohmann::json &j, const std::string &key);
        static std::string getStringOrDefault(const nlohmann::json &j, const std::string &key, const std::string &defaultValue);
        static int getIntOrDefault(const nlohmann::json &j, const std::string &key, int defaultValue);
        static bool getBoolOrDefault(const nlohmann::json &j, const std::string &key, bool defaultValue);

        // Generic value getter
        template<typename T>
        static T Value(const nlohmann::json &j, const std::string &key, const T &defaultValue) {
            return j.contains(key) && j[key].is_null() == false ? j.at(key).get<T>() : defaultValue;
        }
    };

}

#endif // UTILS_JSON_H
