#include "iostream"
#include "producer.h"

rabbitMQ::RabbitMQprocuder::RabbitMQprocuder()
{
}

void rabbitMQ::RabbitMQprocuder::produceMessage(std::string url, std::string exchange, std::string queue, std::string message, std::string key)
{
    AMQP amqp(url);

    rabbitMQexchange = amqp.createExchange(exchange);
    rabbitMQexchange->Declare(exchange, "fanout");

    rabbitMQqueue = amqp.createQueue(queue);
    rabbitMQqueue->Declare();
    rabbitMQqueue->Bind(exchange, "");

    rabbitMQexchange->setHeader("Delivery-mode", 2);
    rabbitMQexchange->setHeader("Content-type", "text/text");
    rabbitMQexchange->setHeader("Content-encoding", "UTF-8");
    rabbitMQexchange->Publish(message, key);
}

rabbitMQ::RabbitMQprocuder::~RabbitMQprocuder() {}
