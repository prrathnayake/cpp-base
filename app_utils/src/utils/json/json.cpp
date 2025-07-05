#include "json.h"
#include <fstream>
#include <stdexcept>

namespace utils
{

    nlohmann::json Json::readFromFile(const std::string &filePath)
    {
        std::ifstream file(filePath);
        if (!file.is_open())
        {
            throw std::runtime_error("Could not open file: " + filePath);
        }

        nlohmann::json j;
        file >> j;
        return j;
    }

    void Json::writeToFile(const std::string &filePath, const nlohmann::json &j)
    {
        std::ofstream file(filePath);
        if (!file.is_open())
        {
            throw std::runtime_error("Could not write to file: " + filePath);
        }

        file << j.dump(4); // Pretty print
    }

    nlohmann::json Json::parse(const std::string &jsonStr)
    {
        return nlohmann::json::parse(jsonStr);
    }

    std::string Json::toString(const nlohmann::json &j)
    {
        return j.dump();
    }
    bool Json::isValidJson(const std::string &jsonStr)
    {
        try
        {
            auto parsed = nlohmann::json::parse(jsonStr);
            return true;
        }
        catch (...)
        {
            return false;
        }
    }

    bool Json::hasKey(const nlohmann::json &j, const std::string &key)
    {
        return j.contains(key);
    }

    std::string Json::getStringOrDefault(const nlohmann::json &j, const std::string &key, const std::string &defaultValue)
    {
        return j.contains(key) && j[key].is_string() ? j[key].get<std::string>() : defaultValue;
    }

    int Json::getIntOrDefault(const nlohmann::json &j, const std::string &key, int defaultValue)
    {
        return j.contains(key) && j[key].is_number_integer() ? j[key].get<int>() : defaultValue;
    }

    bool Json::getBoolOrDefault(const nlohmann::json &j, const std::string &key, bool defaultValue)
    {
        return j.contains(key) && j[key].is_boolean() ? j[key].get<bool>() : defaultValue;
    }

}
