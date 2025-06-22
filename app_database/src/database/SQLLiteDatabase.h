#pragma once
#include <sqlite3.h>
#include <iostream>

namespace database
{
    class SQLLiteDatabase
    {
    public:
        bool connect(const std::string &file);
    };

};
