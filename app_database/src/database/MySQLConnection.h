#pragma once

#include <mysql/mysql.h>
#include <string>

namespace database {

class MySQLConnection {
public:
    MySQLConnection();
    ~MySQLConnection();

    bool connect(const std::string &host, const std::string &user,
                 const std::string &password, const std::string &db = "",
                 unsigned int port = 3306);

    bool selectDatabase(const std::string &db);
    void disconnect();
    bool isConnected() const;
    MYSQL *getRawConnection() const;

private:
    MYSQL *conn_;
};

}