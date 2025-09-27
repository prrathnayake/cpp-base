#include "producer.h"

#include <iostream>
#include <stdexcept>

namespace rabbitMQ
{
    RabbitMQprocuder::RabbitMQprocuder() = default;

    void RabbitMQprocuder::produceMessage(const std::string &url,
                                          const std::string &exchange,
                                          const std::string &queue,
                                          const std::string &message,
                                          const std::string &key)
    {
        AMQP amqp(url);

        rabbitMQexchange = amqp.createExchange(exchange);
        if (!rabbitMQexchange)
        {
            throw std::runtime_error("Failed to create exchange: " + exchange);
        }

        rabbitMQexchange->Declare(exchange, "fanout");

        rabbitMQqueue = amqp.createQueue(queue);
        if (!rabbitMQqueue)
        {
            throw std::runtime_error("Failed to create queue: " + queue);
        }

        rabbitMQqueue->Declare();
        rabbitMQqueue->Bind(exchange, "");

        rabbitMQexchange->setHeader("Delivery-mode", 2);
        rabbitMQexchange->setHeader("Content-type", "text/text");
        rabbitMQexchange->setHeader("Content-encoding", "UTF-8");
        rabbitMQexchange->Publish(message, key);
    }

    RabbitMQprocuder::~RabbitMQprocuder() = default;
}
