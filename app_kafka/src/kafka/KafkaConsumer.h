#pragma once
#include <iostream>
#include <librdkafka/rdkafkacpp.h>

#include "ConsumeCb.h"

namespace kafka
{
    class KafkaConsumer
    {
    public:
        RdKafka::Consumer *consumer;
        RdKafka::Topic *C_topic;
        bool consume = true;

        KafkaConsumer(std::string brokers, std::string topics);

        void consumeMessages(ExCosumeCb ex_consume_cb);
        std::string consumeMessage();


        void stopConsumeMessages();

        ~KafkaConsumer();
    };
}
