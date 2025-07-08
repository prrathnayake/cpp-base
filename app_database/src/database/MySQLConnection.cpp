#include "MySQLConnection.h"
#include <iostream>

namespace database {

MySQLConnection::MySQLConnection() : conn_(mysql_init(nullptr)) {}

MySQLConnection::~MySQLConnection() {
    disconnect();
}

bool MySQLConnection::connect(const std::string &host, const std::string &user,
                              const std::string &password, const std::string &db,
                              unsigned int port) {
    if (!conn_) {
        conn_ = mysql_init(nullptr);
        if (!conn_) {
            std::cerr << "mysql_init failed\n";
            return false;
        }
    }

    if (!mysql_real_connect(conn_, host.c_str(), user.c_str(), password.c_str(),
                            db.empty() ? nullptr : db.c_str(),
                            port, nullptr, 0)) {
        std::cerr << "MySQL connection failed: " << mysql_error(conn_) << "\n";
        return false;
    }

    return true;
}

bool MySQLConnection::selectDatabase(const std::string &db) {
    return conn_ && mysql_select_db(conn_, db.c_str()) == 0;
}

void MySQLConnection::disconnect() {
    if (conn_) {
        mysql_close(conn_);
        conn_ = nullptr;
    }
}

bool MySQLConnection::isConnected() const {
    return conn_ != nullptr;
}

MYSQL *MySQLConnection::getRawConnection() const {
    return conn_;
}

}