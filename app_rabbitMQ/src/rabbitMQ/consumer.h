#pragma once
#include <atomic>
#include <iostream>
#include <memory>

#include <AMQPcpp.h>

namespace rabbitMQ
{
    class RabbitMQconsumer
    {
    public:
        RabbitMQconsumer();
        virtual ~RabbitMQconsumer();

        void consumeMessages(const std::string &url, const std::string &queue);
        virtual void onMessage(const std::string &message);
        void stopConsume();

    private:
        std::unique_ptr<AMQPQueue> rabbitMQqueue;
        std::atomic<bool> consume{true};
    };
}