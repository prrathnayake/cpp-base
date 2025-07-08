#pragma once

#include "MySQLConnection.h"
#include <memory>
#include <queue>
#include <mutex>
#include <condition_variable>

namespace database {

class ConnectionPool {
public:
    ConnectionPool(const std::string& host, const std::string& user,
                   const std::string& password, unsigned int port,
                   size_t poolSize);

    std::shared_ptr<MySQLConnection> getConnection();
    void releaseConnection(std::shared_ptr<MySQLConnection> conn);

private:
    std::queue<std::shared_ptr<MySQLConnection>> pool_;
    std::mutex mtx_;
    std::condition_variable cond_;
};

}