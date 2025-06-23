#pragma once
#include <string>

struct MYSQL;

namespace database
{

    class MySQLDatabase
    {
    public:
        MySQLDatabase();
        ~MySQLDatabase();

        bool connect(const std::string &host, const std::string &user,
                     const std::string &password, const std::string &db, unsigned int port);

        bool isConnected() const;
        void disconnect();

        bool executeUpdate(const std::string &query); // INSERT, UPDATE, DELETE
        bool executeQuery(const std::string &query);  // SELECT
        bool initializeDatabase(const std::string &dbName);
        bool runSqlScript(const std::string &filePath);

        std::string escapeString(const std::string &input);

    private:
        MYSQL *conn;
    };

};
