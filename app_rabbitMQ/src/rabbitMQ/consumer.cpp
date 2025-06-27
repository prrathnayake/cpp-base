#include "iostream"
#include "consumer.h"

rabbitMQ::RabbitMQconsumer::RabbitMQconsumer() {}

rabbitMQ::RabbitMQconsumer::~RabbitMQconsumer() {}

void rabbitMQ::RabbitMQconsumer::consumeMessages(std::string url, std::string queue)
{
    AMQP amqp(url);
    rabbitMQqueue = amqp.createQueue(queue);
    rabbitMQqueue->Declare();

    while (consume)
    {
    again:
        rabbitMQqueue->Get(AMQP_NOACK);

        AMQPMessage *rabbitMQmessage = rabbitMQqueue->getMessage();

        if (rabbitMQmessage->getMessageCount() > -1)
        {
            uint32_t j = 0;
            onMessage(rabbitMQmessage->getMessage(&j));
        }
        else
            goto again;
    }
}

void rabbitMQ::RabbitMQconsumer::onMessage(std::string message)
{
    std::cout << "Message recieved: " << message << std::endl;
}

void rabbitMQ::RabbitMQconsumer::stopConsume()
{
    consume = false;
}
