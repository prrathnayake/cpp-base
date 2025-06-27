#pragma once

#include <string>
#include <nlohmann/json.hpp>

namespace utils {

    class Json {
    public:
        static nlohmann::json readFromFile(const std::string &filePath);
        static void writeToFile(const std::string &filePath, const nlohmann::json &j);
        static nlohmann::json parse(const std::string &jsonStr);
        static std::string toString(const nlohmann::json &j);
    };

}
