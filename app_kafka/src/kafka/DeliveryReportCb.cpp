#include <string>
#include <librdkafka/rdkafkacpp.h>

#include "DeliveryReportCb.h"
#include "utils/log/singletonLogger.h"

void kafka::DeliveryReportCb::dr_cb(RdKafka::Message &message)
{
    if (message.err())
    {
        utils::SingletonLogger::instance().logMeta(
            utils::SingletonLogger::MessageCode::ERROR,
            std::string{"[KafkaProducer] Message delivery failed: "} + message.errstr(),
            __FILE__,
            __LINE__,
            __func__);
        return;
    }

    utils::SingletonLogger::instance().logMeta(
        utils::SingletonLogger::MessageCode::INFO,
        std::string{"[KafkaProducer] Delivered message to topic '"} + message.topic_name() +
            "' at offset " + std::to_string(message.offset()),
        __FILE__,
        __LINE__,
        __func__);
}
