#pragma once
// #include <libpq-fe.h>
#include <iostream>

namespace database
{
    class PostgreDatabase
    {
    public:
        bool connect(const std::string &conn);
    };

};