#include "PostgreDatabase.h"
// #include <libpq-fe.h>
#include <iostream>

namespace database
{
    bool database::PostgreDatabase::connect(const std::string &conn)
    {
        // Use PQconnectdb
        return true;
    };
};
