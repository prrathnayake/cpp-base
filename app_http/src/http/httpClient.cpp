#include "httpClient.h"

#include "utils/log/singletonLogger.h"

HttpClient::HttpClient(const std::string &host, int port)
#ifndef APP_HTTP_MISSING_DEPENDENCIES
    : client(host, port)
#else
    : host_(host), port_(port)
#endif
{
    utils::SingletonLogger::instance().logMeta(
        utils::SingletonLogger::MessageCode::INFO,
        "Initialised HTTP client for " + host + ":" + std::to_string(port),
        __FILE__,
        __LINE__,
        __func__);
}

std::string HttpClient::get(const std::string &path)
{
#ifdef APP_HTTP_MISSING_DEPENDENCIES
    utils::SingletonLogger::instance().logMeta(
        utils::SingletonLogger::MessageCode::WARNING,
        "HTTP GET skipped (httplib missing) for " + host_ + path,
        __FILE__,
        __LINE__,
        __func__);
    return {};
#else
    auto res = client.Get(path.c_str());
    if (res && res->status == 200)
    {
        utils::SingletonLogger::instance().logMeta(
            utils::SingletonLogger::MessageCode::INFO,
            "HTTP GET " + path + " => 200",
            __FILE__,
            __LINE__,
            __func__);
        return res->body;
    }

    utils::SingletonLogger::instance().logMeta(
        utils::SingletonLogger::MessageCode::ERROR,
        "HTTP GET " + path + " failed",
        __FILE__,
        __LINE__,
        __func__);
    return {};
#endif
}

std::string HttpClient::post(const std::string &path, const std::string &body, const std::string &content_type)
{
#ifdef APP_HTTP_MISSING_DEPENDENCIES
    utils::SingletonLogger::instance().logMeta(
        utils::SingletonLogger::MessageCode::WARNING,
        "HTTP POST skipped (httplib missing) for " + host_ + path,
        __FILE__,
        __LINE__,
        __func__);
    return {};
#else
    auto res = client.Post(path.c_str(), body, content_type.c_str());
    if (res && res->status == 200)
    {
        utils::SingletonLogger::instance().logMeta(
            utils::SingletonLogger::MessageCode::INFO,
            "HTTP POST " + path + " => 200",
            __FILE__,
            __LINE__,
            __func__);
        return res->body;
    }

    utils::SingletonLogger::instance().logMeta(
        utils::SingletonLogger::MessageCode::ERROR,
        "HTTP POST " + path + " failed",
        __FILE__,
        __LINE__,
        __func__);
    return {};
#endif
}
