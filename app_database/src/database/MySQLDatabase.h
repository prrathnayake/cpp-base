#pragma once

#include "MySQLConnection.h"
#include <string>

namespace database
{

    class MySQLDatabase
    {
    public:
        MySQLDatabase(const std::string &host, const std::string &user,
                      const std::string &password, unsigned int port = 3306);

        ~MySQLDatabase();

        bool initializeDatabase(const std::string &dbName);
        bool runSqlScript(const std::string &filePath);
        bool executeUpdate(const std::string &query);
        bool executeQuery(const std::string &query);
        std::string escapeString(const std::string &input);

        void setConnection(MySQLConnection *conn);
        MySQLConnection *getConnection() const;

    private:
        MySQLConnection *conn;
        std::string dbName;
    };

} // namespace database
