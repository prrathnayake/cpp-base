#include <string>
#include <librdkafka/rdkafkacpp.h>

#include "ConsumeCb.h"
#include "utils/log/singletonLogger.h"

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
        utils::SingletonLogger::instance().logMeta(
            utils::SingletonLogger::MessageCode::ERROR,
            "Consume failed: " + message->errstr(),
            __FILE__,
            __LINE__,
            __func__);

        break;
    default:
        utils::SingletonLogger::instance().logMeta(
            utils::SingletonLogger::MessageCode::ERROR,
            "Consume failed: " + message->errstr(),
            __FILE__,
            __LINE__,
            __func__);
    }

    return {};
}

void kafka::ExCosumeCb::consume_cb(RdKafka::Message& msg, void* opaque) {
    std::string result = msg_consume(&msg);
    if (!result.empty()) {
        utils::SingletonLogger::instance().logMeta(
            utils::SingletonLogger::MessageCode::INFO,
            "Received message: " + result,
            __FILE__,
            __LINE__,
            __func__);
    }
}