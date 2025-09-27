
#include "MySQLDatabase.h"
#include <fstream>
#include <sstream>

#include "utils/log/singletonLogger.h"

namespace database
{

    MySQLDatabase::MySQLDatabase(const std::string &host, const std::string &user,
                                 const std::string &password, unsigned int port, const std::string &databaseName)
        : host_(host), user_(user), password_(password), port_(port), databaseName_(databaseName)
    {

        pool_ = std::make_shared<ConnectionPool>(host_, user_, password_, port_, databaseName_, 10);
    }

    bool MySQLDatabase::initializeDatabase(const std::string &dbName)
    {
        auto conn = pool_->getConnection();
        databaseName_ = dbName;
        std::string createDbQuery = "CREATE DATABASE IF NOT EXISTS `" + dbName + "`";
        if (mysql_query(conn->getRawConnection(), createDbQuery.c_str()))
        {
            utils::SingletonLogger::instance().logMeta(
                utils::SingletonLogger::MessageCode::ERROR,
                std::string{"Failed to create database: "} + mysql_error(conn->getRawConnection()),
                __FILE__,
                __LINE__,
                __func__);
            return false;
        }

        if (!conn->selectDatabase(dbName))
        {
            utils::SingletonLogger::instance().logMeta(
                utils::SingletonLogger::MessageCode::ERROR,
                std::string{"Failed to select database: "} + mysql_error(conn->getRawConnection()),
                __FILE__,
                __LINE__,
                __func__);
            return false;
        }

        utils::SingletonLogger::instance().logMeta(
            utils::SingletonLogger::MessageCode::INFO,
            "Database '" + dbName + "' initialized and selected.",
            __FILE__,
            __LINE__,
            __func__);
        return true;
    }

    bool MySQLDatabase::executeInsert(const std::string &query)
    {
        auto conn = pool_->getConnection();
        if (mysql_query(conn->getRawConnection(), query.c_str()))
        {
            utils::SingletonLogger::instance().logMeta(
                utils::SingletonLogger::MessageCode::ERROR,
                std::string{"INSERT failed: "} + mysql_error(conn->getRawConnection()),
                __FILE__,
                __LINE__,
                __func__);
            return false;
        }
        utils::SingletonLogger::instance().logMeta(
            utils::SingletonLogger::MessageCode::INFO,
            "INSERT query executed successfully.",
            __FILE__,
            __LINE__,
            __func__);
        return true;
    }

    bool MySQLDatabase::executeUpdate(const std::string &query)
    {
        auto conn = pool_->getConnection();
        if (mysql_query(conn->getRawConnection(), query.c_str()))
        {
            utils::SingletonLogger::instance().logMeta(
                utils::SingletonLogger::MessageCode::ERROR,
                std::string{"UPDATE failed: "} + mysql_error(conn->getRawConnection()),
                __FILE__,
                __LINE__,
                __func__);
            return false;
        }
        utils::SingletonLogger::instance().logMeta(
            utils::SingletonLogger::MessageCode::INFO,
            "UPDATE query executed successfully.",
            __FILE__,
            __LINE__,
            __func__);
        return true;
    }

    bool MySQLDatabase::executeDelete(const std::string &query)
    {
        auto conn = pool_->getConnection();
        if (mysql_query(conn->getRawConnection(), query.c_str()))
        {
            utils::SingletonLogger::instance().logMeta(
                utils::SingletonLogger::MessageCode::ERROR,
                std::string{"DELETE failed: "} + mysql_error(conn->getRawConnection()),
                __FILE__,
                __LINE__,
                __func__);
            return false;
        }
        utils::SingletonLogger::instance().logMeta(
            utils::SingletonLogger::MessageCode::INFO,
            "DELETE query executed successfully.",
            __FILE__,
            __LINE__,
            __func__);
        return true;
    }

    bool MySQLDatabase::executeSelect(const std::string &query)
    {
        auto conn = pool_->getConnection();
        if (mysql_query(conn->getRawConnection(), query.c_str()))
        {
            utils::SingletonLogger::instance().logMeta(
                utils::SingletonLogger::MessageCode::ERROR,
                std::string{"SELECT failed: "} + mysql_error(conn->getRawConnection()),
                __FILE__,
                __LINE__,
                __func__);
            return false;
        }

        MYSQL_RES *result = mysql_store_result(conn->getRawConnection());
        if (!result)
        {
            utils::SingletonLogger::instance().logMeta(
                utils::SingletonLogger::MessageCode::ERROR,
                std::string{"Failed to retrieve SELECT result: "} + mysql_error(conn->getRawConnection()),
                __FILE__,
                __LINE__,
                __func__);
            return false;
        }

        int num_fields = mysql_num_fields(result);
        MYSQL_ROW row;

        while ((row = mysql_fetch_row(result)))
        {
            std::string rowData;
            for (int i = 0; i < num_fields; ++i)
            {
                if (i != 0)
                {
                    rowData += ' ';
                }
                rowData += (row[i] ? row[i] : "NULL");
            }
            utils::SingletonLogger::instance().logMeta(
                utils::SingletonLogger::MessageCode::INFO,
                "SELECT row: " + rowData,
                __FILE__,
                __LINE__,
                __func__);
        }

        mysql_free_result(result);
        return true;
    }

    std::vector<std::map<std::string, std::string>> MySQLDatabase::fetchRows(const std::string &query)
    {
        auto conn = pool_->getConnection();
        std::vector<std::map<std::string, std::string>> results;

        if (mysql_query(conn->getRawConnection(), query.c_str()))
        {
            utils::SingletonLogger::instance().logMeta(
                utils::SingletonLogger::MessageCode::ERROR,
                std::string{"SELECT failed: "} + mysql_error(conn->getRawConnection()),
                __FILE__,
                __LINE__,
                __func__);
            return results;
        }

        MYSQL_RES *result = mysql_store_result(conn->getRawConnection());
        if (!result)
        {
            utils::SingletonLogger::instance().logMeta(
                utils::SingletonLogger::MessageCode::ERROR,
                std::string{"Failed to retrieve SELECT result: "} + mysql_error(conn->getRawConnection()),
                __FILE__,
                __LINE__,
                __func__);
            return results;
        }

        int num_fields = mysql_num_fields(result);
        MYSQL_ROW row;
        MYSQL_FIELD *fields = mysql_fetch_fields(result);

        while ((row = mysql_fetch_row(result)))
        {
            std::map<std::string, std::string> record;
            for (int i = 0; i < num_fields; ++i)
            {
                std::string key = fields[i].name;
                std::string value = row[i] ? row[i] : "";
                record[key] = value;
            }
            results.push_back(record);
        }

        mysql_free_result(result);
        return results;
    }

    bool MySQLDatabase::runSqlScript(const std::string &filePath)
    {
        auto conn = pool_->getConnection();

        std::ifstream file(filePath);
        if (!file.is_open())
        {
            utils::SingletonLogger::instance().logMeta(
                utils::SingletonLogger::MessageCode::ERROR,
                "Could not open SQL script: " + filePath,
                __FILE__,
                __LINE__,
                __func__);
            return false;
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string script = buffer.str();

        auto trim = [](const std::string &s) -> std::string
        {
            size_t start = s.find_first_not_of(" 	\n\r");
            if (start == std::string::npos)
                return "";
            size_t end = s.find_last_not_of(" 	\n\r");
            return s.substr(start, end - start + 1);
        };

        size_t pos;
        while ((pos = script.find(';')) != std::string::npos)
        {
            std::string statement = trim(script.substr(0, pos));
            script.erase(0, pos + 1);
            if (!statement.empty())
            {
                if (mysql_query(conn->getRawConnection(), statement.c_str()))
                {
                    utils::SingletonLogger::instance().logMeta(
                        utils::SingletonLogger::MessageCode::ERROR,
                        std::string{"Failed to execute SQL: "} + mysql_error(conn->getRawConnection()),
                        __FILE__,
                        __LINE__,
                        __func__);
                    utils::SingletonLogger::instance().logMeta(
                        utils::SingletonLogger::MessageCode::ERROR,
                        "While executing: " + statement,
                        __FILE__,
                        __LINE__,
                        __func__);
                    return false;
                }
            }
        }

        std::string lastStatement = trim(script);
        if (!lastStatement.empty())
        {
            if (mysql_query(conn->getRawConnection(), lastStatement.c_str()))
            {
                utils::SingletonLogger::instance().logMeta(
                    utils::SingletonLogger::MessageCode::ERROR,
                    std::string{"Failed to execute SQL: "} + mysql_error(conn->getRawConnection()),
                    __FILE__,
                    __LINE__,
                    __func__);
                utils::SingletonLogger::instance().logMeta(
                    utils::SingletonLogger::MessageCode::ERROR,
                    "While executing: " + lastStatement,
                    __FILE__,
                    __LINE__,
                    __func__);
                return false;
            }
        }

        utils::SingletonLogger::instance().logMeta(
            utils::SingletonLogger::MessageCode::INFO,
            "SQL script executed successfully.",
            __FILE__,
            __LINE__,
            __func__);
        return true;
    }

    std::string MySQLDatabase::escapeString(const std::string &input)
    {
        auto conn = pool_->getConnection();
        if (!conn || !conn->getRawConnection())
        {
            throw std::runtime_error("Invalid MySQL connection");
        }
        std::string escaped;
        escaped.resize(input.size() * 2 + 1);
        unsigned long len = mysql_real_escape_string(conn->getRawConnection(), &escaped[0], input.c_str(), input.length());
        escaped.resize(len);
        return escaped;
    }

}
