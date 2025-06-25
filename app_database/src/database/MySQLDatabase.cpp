#include "MySQLDatabase.h"
#include <iostream>
#include <fstream>
#include <sstream>

namespace database {

MySQLDatabase::MySQLDatabase(const std::string &host, const std::string &user,
                             const std::string &password, unsigned int port) {
    conn = new MySQLConnection();
    if (!conn->connect(host, user, password, "", port)) {
        std::cerr << "Initial connection to MySQL server failed.\n";
    }
}

MySQLDatabase::~MySQLDatabase() {
    if (conn) {
        conn->disconnect();
        delete conn;
        conn = nullptr;
    }
}

void MySQLDatabase::setConnection(MySQLConnection *connection) {
    conn = connection;
}

MySQLConnection *MySQLDatabase::getConnection() const {
    return conn;
}

bool MySQLDatabase::initializeDatabase(const std::string &db) {
    if (!conn || !conn->isConnected()) {
        std::cerr << "No active MySQL connection.\n";
        return false;
    }

    dbName = db;
    std::string createDbQuery = "CREATE DATABASE IF NOT EXISTS `" + db + "`";

    if (mysql_query(conn->getRawConnection(), createDbQuery.c_str())) {
        std::cerr << "Failed to create database: " << mysql_error(conn->getRawConnection()) << "\n";
        return false;
    }

    if (!conn->selectDatabase(db)) {
        std::cerr << "Failed to select database: " << mysql_error(conn->getRawConnection()) << "\n";
        return false;
    }

    std::cout << "Database '" << db << "' initialized and selected.\n";
    return true;
}

bool MySQLDatabase::executeUpdate(const std::string &query) {
    if (!conn || !conn->isConnected()) {
        std::cerr << "No active MySQL connection.\n";
        return false;
    }

    if (mysql_query(conn->getRawConnection(), query.c_str())) {
        std::cerr << "MySQL update failed: " << mysql_error(conn->getRawConnection()) << "\n";
        return false;
    }

    return true;
}

bool MySQLDatabase::executeQuery(const std::string &query) {
    if (!conn || !conn->isConnected()) {
        std::cerr << "No active MySQL connection.\n";
        return false;
    }

    if (mysql_query(conn->getRawConnection(), query.c_str())) {
        std::cerr << "MySQL query failed: " << mysql_error(conn->getRawConnection()) << "\n";
        return false;
    }

    MYSQL_RES *result = mysql_store_result(conn->getRawConnection());
    if (!result) {
        std::cerr << "Failed to store result: " << mysql_error(conn->getRawConnection()) << "\n";
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

bool MySQLDatabase::runSqlScript(const std::string &filePath) {
    if (!conn || !conn->isConnected()) {
        std::cerr << "No active MySQL connection.\n";
        return false;
    }

    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Could not open SQL script: " << filePath << "\n";
        return false;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string script = buffer.str();

    auto trim = [](const std::string &s) -> std::string {
        size_t start = s.find_first_not_of(" \t\n\r");
        if (start == std::string::npos)
            return "";
        size_t end = s.find_last_not_of(" \t\n\r");
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
    if (!conn || !conn->isConnected()) {
        return "";
    }

    std::string escaped;
    escaped.resize(input.size() * 2 + 1);
    unsigned long len = mysql_real_escape_string(conn->getRawConnection(), &escaped[0], input.c_str(), input.length());
    escaped.resize(len);
    return escaped;
}

} // namespace database
