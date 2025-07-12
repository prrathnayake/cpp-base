#include <iostream>
#include <string>
#include <librdkafka/rdkafkacpp.h>

#include "ConsumeCb.h"

std::string kafka::ExCosumeCb::msg_consume(RdKafka::Message *message)
{
    switch (message->err())
    {
    case RdKafka::ERR__TIMED_OUT:
        break;
    case RdKafka::ERR_NO_ERROR:
        return std::string(static_cast<const char *>(message->payload()), message->len());

    case RdKafka::ERR__UNKNOWN_TOPIC:
    case RdKafka::ERR__UNKNOWN_PARTITION:
        std::cerr << "Consume failed: " << message->errstr() << std::endl;

        break;
    default:
        std::cerr << "Consume failed: " << message->errstr() << std::endl;
    }
}

void kafka::ExCosumeCb::consume_cb(RdKafka::Message& msg, void* opaque) {
    std::string result = msg_consume(&msg);
    if (!result.empty()) {
        std::cout << "Received message: " << result << std::endl;
    }
}