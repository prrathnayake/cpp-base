#pragma once
#include "iostream"
#include <AMQPcpp.h>

namespace rabbitMQ
{
    class RabbitMQprocuder
    {
    private:
        AMQPExchange *rabbitMQexchange;
        AMQPQueue *rabbitMQqueue;

    public:
        RabbitMQprocuder();
        void produceMessage(std::string url, std::string exchange, std::string queue, std::string message, std::string key);
        ~RabbitMQprocuder();
    };
}