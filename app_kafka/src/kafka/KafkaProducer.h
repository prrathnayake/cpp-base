#pragma once
#include <iostream>
#include <string>
#include <librdkafka/rdkafkacpp.h>

namespace kafka
{
    class KafkaProducer
    {
    public:
        RdKafka::Producer *producer;

        KafkaProducer(std::string brokers);

        void produceMessages(std::string topic, std::string message);

        ~KafkaProducer();
    };
}
