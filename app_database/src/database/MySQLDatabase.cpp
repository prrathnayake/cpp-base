#include "MySQLDatabase.h"
#include <mysql/mysql.h>
#include <iostream>
#include <fstream>   // std::ifstream
#include <sstream>   // std::stringstream, std::istringstream
#include <algorithm> // std::remove_copy_if
#include <iterator>  // std::back_inserter
#include <cctype>    // std::isspace

namespace database
{

    MySQLDatabase::MySQLDatabase() : conn(mysql_init(nullptr)) {}

    MySQLDatabase::~MySQLDatabase()
    {
        if (conn)
        {
            mysql_close(conn);
        }
    }

    bool MySQLDatabase::connect(const std::string &host, const std::string &user,
                                const std::string &password, const std::string &db, unsigned int port)
    {
        if (!conn)
        {
            conn = mysql_init(nullptr);
            if (!conn)
            {
                std::cerr << "MySQL init failed\n";
                return false;
            }
        }

        const char *dbname = db.empty() ? nullptr : db.c_str();

        if (!mysql_real_connect(conn, host.c_str(), user.c_str(), password.c_str(), dbname, port, nullptr, 0))
        {
            std::cerr << "MySQL connection failed: " << mysql_error(conn) << "\n";
            return false;
        }

        std::cout << "Connected to MySQL server.\n";
        return true;
    }

    bool MySQLDatabase::isConnected() const
    {
        return conn != nullptr;
    }

    void MySQLDatabase::disconnect()
    {
        if (conn)
        {
            mysql_close(conn);
            conn = nullptr;
            std::cout << "Disconnected from MySQL database.\n";
        }
    }

    bool MySQLDatabase::executeUpdate(const std::string &query)
    {
        if (!conn)
        {
            std::cerr << "No active MySQL connection.\n";
            return false;
        }

        if (mysql_query(conn, query.c_str()))
        {
            std::cerr << "MySQL update failed: " << mysql_error(conn) << "\n";
            return false;
        }

        std::cout << "Query executed successfully.\n";
        return true;
    }

    bool MySQLDatabase::executeQuery(const std::string &query)
    {
        if (!conn)
        {
            std::cerr << "No active MySQL connection.\n";
            return false;
        }

        if (mysql_query(conn, query.c_str()))
        {
            std::cerr << "MySQL query failed: " << mysql_error(conn) << "\n";
            return false;
        }

        MYSQL_RES *result = mysql_store_result(conn);
        if (!result)
        {
            std::cerr << "Failed to store result: " << mysql_error(conn) << "\n";
            return false;
        }

        int num_fields = mysql_num_fields(result);
        MYSQL_ROW row;

        while ((row = mysql_fetch_row(result)))
        {
            for (int i = 0; i < num_fields; ++i)
            {
                std::cout << (row[i] ? row[i] : "NULL") << " ";
            }
            std::cout << "\n";
        }

        mysql_free_result(result);
        return true;
    }

    bool MySQLDatabase::initializeDatabase(const std::string &dbName)
    {
        // Connect without specifying a database
        //     if (!mysql_real_connect(conn, "localhost", "root", "your_password", nullptr, 3306, nullptr, 0))
        //     {
        //         std::cerr << "Connection failed: " << mysql_error(conn) << "\n";
        //         return false;
        //     }

        //     // Create database if not exists
        //     std::string createDbQuery = "CREATE DATABASE IF NOT EXISTS " + dbName;
        //     if (mysql_query(conn, createDbQuery.c_str()))
        //     {
        //         std::cerr << "Failed to create database: " << mysql_error(conn) << "\n";
        //         return false;
        //     }

        //     // Select the database
        //     if (mysql_select_db(conn, dbName.c_str()))
        //     {
        //         std::cerr << "Failed to select database: " << mysql_error(conn) << "\n";
        //         return false;
        //     }

        //     // Create tables if they don’t exist
        //     std::string createUserTable = R"(
        //     CREATE TABLE IF NOT EXISTS users (
        //         id INT AUTO_INCREMENT PRIMARY KEY,
        //         username VARCHAR(50) NOT NULL UNIQUE,
        //         email VARCHAR(100) NOT NULL UNIQUE,
        //         password_hash VARCHAR(255) NOT NULL,
        //         created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
        //     )
        // )";

        //     if (mysql_query(conn, createUserTable.c_str()))
        //     {
        //         std::cerr << "Failed to create users table: " << mysql_error(conn) << "\n";
        //         return false;
        //     }

        //     std::cout << "Database and table initialization completed.\n";
        return true;
    }

    bool MySQLDatabase::runSqlScript(const std::string &filePath)
    {
        // Open the SQL file
        std::ifstream file(filePath);
        if (!file.is_open())
        {
            std::cerr << "Could not open SQL script: " << filePath << "\n";
            return false;
        }

        // Read entire file into a string
        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string script = buffer.str();

        // Helper lambda to trim whitespace from both ends
        auto trim = [](const std::string &s) -> std::string
        {
            size_t start = s.find_first_not_of(" \t\n\r");
            if (start == std::string::npos)
                return ""; // all whitespace
            size_t end = s.find_last_not_of(" \t\n\r");
            return s.substr(start, end - start + 1);
        };

        size_t pos = 0;
        while ((pos = script.find(';')) != std::string::npos)
        {
            std::string statement = script.substr(0, pos);
            script.erase(0, pos + 1);

            statement = trim(statement);
            if (!statement.empty())
            {
                if (mysql_query(conn, statement.c_str()))
                {
                    std::cerr << "Failed to execute SQL: " << mysql_error(conn) << "\n";
                    std::cerr << "While executing: " << statement << "\n";
                    return false;
                }
            }
        }

        // Check and run any remaining statement after last semicolon
        std::string lastStatement = trim(script);
        if (!lastStatement.empty())
        {
            if (mysql_query(conn, lastStatement.c_str()))
            {
                std::cerr << "Failed to execute SQL: " << mysql_error(conn) << "\n";
                std::cerr << "While executing: " << lastStatement << "\n";
                return false;
            }
        }

        std::cout << "SQL script executed successfully.\n";
        return true;
    }

    std::string database::MySQLDatabase::escapeString(const std::string &input)
    {
        if (!conn)
        {
            return ""; // or throw/log if needed
        }

        std::string escaped;
        escaped.resize(input.size() * 2 + 1); // max needed space
        unsigned long len = mysql_real_escape_string(conn, &escaped[0], input.c_str(), input.length());
        escaped.resize(len); // shrink to actual size
        return escaped;
    }

};
