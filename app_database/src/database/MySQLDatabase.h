#pragma once
#include <mysql/mysql.h>
#include <iostream>

namespace database
{
    class MySQLDatabase
    {
    public:
        MySQLDatabase();
        ~MySQLDatabase();

        bool connect(const std::string &host, const std::string &user, const std::string &password, const std::string &db, unsigned int port);

    private:
        MYSQL *conn;
    };

}
