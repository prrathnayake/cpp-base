
#include "MySQLDatabase.h"
#include <iostream>
#include <fstream>
#include <sstream>

namespace database {

    MySQLDatabase::MySQLDatabase(const std::string &host, const std::string &user,
                                 const std::string &password, unsigned int port)
        : host_(host), user_(user), password_(password), port_(port), databaseName_("") {
        conn = new MySQLConnection();
        if (!conn->connect(host, user, password, "", port)) {
            std::cerr << "Initial connection to MySQL server failed.\n";
        }
    }

    MySQLDatabase::~MySQLDatabase() {
        disconnect();
    }

    void MySQLDatabase::connect() {
        if (!conn || !conn->isConnected()) {
            conn = new MySQLConnection();
            if (!conn->connect(host_, user_, password_, databaseName_, port_)) {
                std::cerr << "Reconnection to MySQL server failed.\n";
            }
        }
    }

    void MySQLDatabase::disconnect() {
        if (conn) {
            conn->disconnect();
            delete conn;
            conn = nullptr;
        }
    }

    bool MySQLDatabase::initializeDatabase(const std::string &dbName) {
        databaseName_ = dbName;
        std::string createDbQuery = "CREATE DATABASE IF NOT EXISTS `" + dbName + "`";
        if (mysql_query(conn->getRawConnection(), createDbQuery.c_str())) {
            std::cerr << "Failed to create database: " << mysql_error(conn->getRawConnection()) << "\n";
            return false;
        }

        if (!conn->selectDatabase(dbName)) {
            std::cerr << "Failed to select database: " << mysql_error(conn->getRawConnection()) << "\n";
            return false;
        }

        std::cout << "Database '" << dbName << "' initialized and selected.\n";
        return true;
    }

    void MySQLDatabase::setConnection(MySQLConnection *connection) {
        conn = connection;
    }

    MySQLConnection *MySQLDatabase::getConnection() const {
        return conn;
    }

    bool MySQLDatabase::executeInsert(const std::string &query) {
        connect();
        if (mysql_query(conn->getRawConnection(), query.c_str())) {
            std::cerr << "INSERT failed: " << mysql_error(conn->getRawConnection()) << "\n";
            return false;
        }
        std::cout << "INSERT query executed successfully.\n";
        return true;
    }

    bool MySQLDatabase::executeUpdate(const std::string &query) {
        connect();
        if (mysql_query(conn->getRawConnection(), query.c_str())) {
            std::cerr << "UPDATE failed: " << mysql_error(conn->getRawConnection()) << "\n";
            return false;
        }
        std::cout << "UPDATE query executed successfully.\n";
        return true;
    }

    bool MySQLDatabase::executeDelete(const std::string &query) {
        connect();
        if (mysql_query(conn->getRawConnection(), query.c_str())) {
            std::cerr << "DELETE failed: " << mysql_error(conn->getRawConnection()) << "\n";
            return false;
        }
        std::cout << "DELETE query executed successfully.\n";
        return true;
    }

    bool MySQLDatabase::executeSelect(const std::string &query) {
        connect();
        if (mysql_query(conn->getRawConnection(), query.c_str())) {
            std::cerr << "SELECT failed: " << mysql_error(conn->getRawConnection()) << "\n";
            return false;
        }

        MYSQL_RES *result = mysql_store_result(conn->getRawConnection());
        if (!result) {
            std::cerr << "Failed to retrieve SELECT result: " << mysql_error(conn->getRawConnection()) << "\n";
            return false;
        }

        int num_fields = mysql_num_fields(result);
        MYSQL_ROW row;

        while ((row = mysql_fetch_row(result))) {
            for (int i = 0; i < num_fields; ++i) {
                std::cout << (row[i] ? row[i] : "NULL") << " ";
            }
            std::cout << "\n";
        }

        mysql_free_result(result);
        return true;
    }

    std::vector<std::map<std::string, std::string>> MySQLDatabase::fetchRows(const std::string &query) {
    connect();
    std::vector<std::map<std::string, std::string>> results;

    if (mysql_query(conn->getRawConnection(), query.c_str())) {
        std::cerr << "SELECT failed: " << mysql_error(conn->getRawConnection()) << "\n";
        return results;
    }

    MYSQL_RES *result = mysql_store_result(conn->getRawConnection());
    if (!result) {
        std::cerr << "Failed to retrieve SELECT result: " << mysql_error(conn->getRawConnection()) << "\n";
        return results;
    }

    int num_fields = mysql_num_fields(result);
    MYSQL_ROW row;
    MYSQL_FIELD *fields = mysql_fetch_fields(result);

    while ((row = mysql_fetch_row(result))) {
        std::map<std::string, std::string> record;
        for (int i = 0; i < num_fields; ++i) {
            std::string key = fields[i].name;
            std::string value = row[i] ? row[i] : "";
            record[key] = value;
        }
        results.push_back(record);
    }

    mysql_free_result(result);
    return results;
}


    bool MySQLDatabase::runSqlScript(const std::string &filePath) {
        connect();

        std::ifstream file(filePath);
        if (!file.is_open()) {
            std::cerr << "Could not open SQL script: " << filePath << "\n";
            return false;
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string script = buffer.str();

        auto trim = [](const std::string &s) -> std::string {
            size_t start = s.find_first_not_of(" 	\n\r");
            if (start == std::string::npos) return "";
            size_t end = s.find_last_not_of(" 	\n\r");
            return s.substr(start, end - start + 1);
        };

        size_t pos;
        while ((pos = script.find(';')) != std::string::npos) {
            std::string statement = trim(script.substr(0, pos));
            script.erase(0, pos + 1);
            if (!statement.empty()) {
                if (mysql_query(conn->getRawConnection(), statement.c_str())) {
                    std::cerr << "Failed to execute SQL: " << mysql_error(conn->getRawConnection()) << "\n";
                    std::cerr << "While executing: " << statement << "\n";
                    return false;
                }
            }
        }

        std::string lastStatement = trim(script);
        if (!lastStatement.empty()) {
            if (mysql_query(conn->getRawConnection(), lastStatement.c_str())) {
                std::cerr << "Failed to execute SQL: " << mysql_error(conn->getRawConnection()) << "\n";
                std::cerr << "While executing: " << lastStatement << "\n";
                return false;
            }
        }

        std::cout << "SQL script executed successfully.\n";
        return true;
    }

    std::string MySQLDatabase::escapeString(const std::string &input) {
        connect();
        std::string escaped;
        escaped.resize(input.size() * 2 + 1);
        unsigned long len = mysql_real_escape_string(conn->getRawConnection(), &escaped[0], input.c_str(), input.length());
        escaped.resize(len);
        return escaped;
    }

} // namespace database
