#include "MySQLConnection.h"
#include <iostream>

namespace database {

MySQLConnection::MySQLConnection()
    : conn(mysql_init(nullptr)) {}

MySQLConnection::~MySQLConnection() {
    disconnect();
}

bool MySQLConnection::connect(const std::string &host, const std::string &user,
                              const std::string &password, const std::string &db,
                              unsigned int port) {
    if (!conn) {
        conn = mysql_init(nullptr);
        if (!conn) {
            std::cerr << "mysql_init failed\n";
            return false;
        }
    }

    if (!mysql_real_connect(conn, host.c_str(), user.c_str(), password.c_str(),
                            db.empty() ? nullptr : db.c_str(),
                            port, nullptr, 0)) {
        std::cerr << "MySQL connection failed: " << mysql_error(conn) << "\n";
        return false;
    }

    return true;
}

bool MySQLConnection::selectDatabase(const std::string &db) {
    if (!conn) return false;
    return mysql_select_db(conn, db.c_str()) == 0;
}

void MySQLConnection::disconnect() {
    if (conn) {
        mysql_close(conn);
        conn = nullptr;
    }
}

bool MySQLConnection::isConnected() const {
    return conn != nullptr;
}

MYSQL *MySQLConnection::getRawConnection() const {
    return conn;
}

} // namespace database
