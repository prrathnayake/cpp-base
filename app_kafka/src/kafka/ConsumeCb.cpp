#include <iostream>
#include <string>
#include <librdkafka/rdkafkacpp.h>

#include "ConsumeCb.h"

void kafka::ExCosumeCb::msg_consume(RdKafka::Message *message)
{
    switch (message->err())
    {
    case RdKafka::ERR__TIMED_OUT:
        break;
    case RdKafka::ERR_NO_ERROR:
        std::cout << std::stoll(static_cast<const char *>(message->payload())) << std::endl;
        break;

    case RdKafka::ERR__UNKNOWN_TOPIC:
    case RdKafka::ERR__UNKNOWN_PARTITION:
        std::cerr << "Consume failed: " << message->errstr() << std::endl;

        break;
    default:
        std::cerr << "Consume failed: " << message->errstr() << std::endl;
    }
}

void kafka::ExCosumeCb::consume_cb(RdKafka::Message &msg, void *opaque)
{

    msg_consume(&msg);
}
