#pragma once
#include <iostream>
#include <string>
#include <librdkafka/rdkafkacpp.h>

namespace kafka
{
    class ExCosumeCb : public RdKafka::ConsumeCb
    {
    public:
        std::string msg_consume(RdKafka::Message *message);

        void consume_cb(RdKafka::Message &msg, void *opaque);
    };
}
