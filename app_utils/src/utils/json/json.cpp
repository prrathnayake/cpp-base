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

}
