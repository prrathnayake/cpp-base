#pragma once
#include <iostream>
#include <unordered_map>

namespace blockchain
{
    class Database
    {
    private:
        std::unordered_map<std::string, std::string> referenceDataMap;

    public:
        void addData(std::string, std::string);
        int size();
        void printData();
    };
}