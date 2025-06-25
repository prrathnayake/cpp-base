#pragma once
#include <httplib.h>
#include <string>

namespace http
{
    class HttpServer
    {
    public:
        HttpServer(const std::string &host = "localhost", int port = 1234);

        void start();
        void stop();

        httplib::Server &getServer(); // <-- expose server for route setup

    private:
        httplib::Server server;
        std::string host;
        int port;
    };
}
