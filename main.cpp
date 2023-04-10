#include <iostream>
#include <chrono>
#include <thread>

#include <utils/time.h>
#include <utils/ThreadPool.h>
#include <rabbitMQ/producer.h>
#include <rabbitMQ/consumer.h>

void sendMessage(rabbitMQ::RabbitMQprocuder *rabbitMQproducer, std::string url, std::string exchange, std::string queue)
{
    for (int i= 0; i < 5; i++)
    {
        rabbitMQproducer->produceMessage(url, exchange, queue, "Hello", "key");
    }
}

void consumeMessage(rabbitMQ::RabbitMQconsumer *rabbitMQconsumer, std::string url, std::string queue)
{
    rabbitMQconsumer->consumeMessages(url, queue);
}

int main()
{
    utils::ThreadPool pool;
    pool.addThread("consumer");
    pool.addThread("producer");

    std::string url = "fhmsdkaj:mV5nq8WyrPVOvJhCmgrnm0c2ZxGhl03p@vulture.rmq.cloudamqp.com:5672/fhmsdkaj";
    std::string exchange = "exchange";
    std::string queue = "queue";

    rabbitMQ::RabbitMQconsumer *rabbitMQconsumer = new rabbitMQ::RabbitMQconsumer();
    pool.addTask("consumer", std::bind(consumeMessage, rabbitMQconsumer, url, queue));

    utils::Time::holdThread(5);

    rabbitMQ::RabbitMQprocuder *rabbitMQproducer = new rabbitMQ::RabbitMQprocuder();
    pool.addTask("producer", std::bind(sendMessage, rabbitMQproducer, url, exchange, queue));

    utils::Time::holdThread(20);

    pool.joinAll();
    return 0;
}