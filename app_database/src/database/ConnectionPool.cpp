#include "ConnectionPool.h"
#include <stdexcept>

namespace database
{

    ConnectionPool::ConnectionPool(const std::string &host, const std::string &user,
                                   const std::string &password, unsigned int port, const std::string &databaseName,
                                   size_t poolSize)
    {
        for (size_t i = 0; i < poolSize; ++i)
        {
            auto conn = std::make_shared<MySQLConnection>();
            if (conn->connect(host, user, password, databaseName, port))
            {
                pool_.push(conn);
            }
            else
            {
                throw std::runtime_error("Failed to create MySQL connection for pool.");
            }
        }
    }

    std::shared_ptr<MySQLConnection> ConnectionPool::getConnection()
    {
        std::unique_lock<std::mutex> lock(mtx_);
        cond_.wait(lock, [&]()
                   { return !pool_.empty(); });
        auto conn = pool_.front();
        pool_.pop();
        return conn;
    }

    void ConnectionPool::releaseConnection(std::shared_ptr<MySQLConnection> conn)
    {
        std::lock_guard<std::mutex> lock(mtx_);
        pool_.push(conn);
        cond_.notify_one();
    }

}