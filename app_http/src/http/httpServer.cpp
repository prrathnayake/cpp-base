#include "httpServer.h"

http::HttpServer::HttpServer(const std::string& host, int port) : host(host), port(port) {}

httplib::Server& http::HttpServer::getServer() {
    return server;
}

void http::HttpServer::start() {
    server.listen(host.c_str(), port);
}

void http::HttpServer::stop() {
    server.stop();
}
