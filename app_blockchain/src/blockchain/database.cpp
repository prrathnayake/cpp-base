#include <iostream>
#include "database.h"

void blockchain::Database::addData(std::string key, std::string value)
{
    referenceDataMap[key] = value;
}

int blockchain::Database::size()
{
    return referenceDataMap.size();
}

void blockchain::Database::printData()
{
    std::cout << "Database | size = " << size() << "\n";
    for (auto &pair :
         referenceDataMap)
    {
        std::cout << pair.first << " : " << pair.second << "\n";
    }
    std::cout << "\n";
}
