#pragma once
#include <string>

#ifndef APP_HTTP_MISSING_DEPENDENCIES
#include <httplib.h>
#endif

class HttpClient
{
public:
    HttpClient(const std::string &host, int port);

    std::string get(const std::string &path);
    std::string post(const std::string &path, const std::string &body, const std::string &content_type = "text/plain");

private:
#ifdef APP_HTTP_MISSING_DEPENDENCIES
    std::string host_;
    int port_{};
#else
    httplib::Client client;
#endif
};
