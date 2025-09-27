#pragma once
#include <iostream>
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
        void produceMessage(const std::string &url,
                            const std::string &exchange,
                            const std::string &queue,
                            const std::string &message,
                            const std::string &key);
        ~RabbitMQprocuder();
    };
}