#include "MySQLDatabase.h"
#include <mysql/mysql.h>
#include <iostream>

database::MySQLDatabase::MySQLDatabase() : conn(mysql_init(nullptr)) {};

database::MySQLDatabase::~MySQLDatabase()
{
    if (conn)
    {
        mysql_close(conn);
    };
};

bool database::MySQLDatabase::connect(const std::string &host, const std::string &user, const std::string &password, const std::string &db, unsigned int port)
{
    if (!conn)
    {
        std::cerr << "MySQL init failed\n";
        return false;
    }

    if (!mysql_real_connect(conn, host.c_str(), user.c_str(), password.c_str(), db.c_str(), port, nullptr, 0))
    {
        std::cerr << "MySQL connection failed: " << mysql_error(conn) << "\n";
        return false;
    }

    std::cout << "Connected to MySQL database.\n";
    return true;
};