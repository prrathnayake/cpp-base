
#pragma once

#include "MySQLConnection.h"
#include <string>
#include <vector>
#include <map>

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

        bool executeInsert(const std::string &query);
        bool executeUpdate(const std::string &query);
        bool executeDelete(const std::string &query);
        bool executeSelect(const std::string &query);
        std::vector<std::map<std::string, std::string>> fetchRows(const std::string &query);

        std::string escapeString(const std::string &input);

        void setConnection(MySQLConnection *conn);
        MySQLConnection *getConnection() const;

        void connect();
        void disconnect();

    private:
        MySQLConnection *conn;
        std::string host_;
        std::string user_;
        std::string password_;
        std::string databaseName_;
        unsigned int port_;
    };

} // namespace database
