#include "consumer.h"

#include <stdexcept>

namespace rabbitMQ
{
    RabbitMQconsumer::RabbitMQconsumer() = default;

    RabbitMQconsumer::~RabbitMQconsumer() = default;

    void RabbitMQconsumer::consumeMessages(const std::string &url, const std::string &queue)
    {
        AMQP amqp(url);
        rabbitMQqueue.reset(amqp.createQueue(queue));
        if (!rabbitMQqueue)
        {
            throw std::runtime_error("Failed to create queue: " + queue);
        }

        rabbitMQqueue->Declare();

        while (consume.load(std::memory_order_acquire))
        {
            rabbitMQqueue->Get(AMQP_NOACK);

            AMQPMessage *rabbitMQmessage = rabbitMQqueue->getMessage();
            if (!rabbitMQmessage)
            {
                continue;
            }

            if (rabbitMQmessage->getMessageCount() > -1)
            {
                uint32_t j = 0;
                onMessage(rabbitMQmessage->getMessage(&j));
            }
        }
    }

    void RabbitMQconsumer::onMessage(const std::string &message)
    {
        std::cout << "Message received: " << message << std::endl;
    }

    void RabbitMQconsumer::stopConsume()
    {
        consume.store(false, std::memory_order_release);
    }
}
