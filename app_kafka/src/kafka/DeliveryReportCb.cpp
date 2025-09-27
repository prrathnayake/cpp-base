#include <iostream>
#include <string>
#include <librdkafka/rdkafkacpp.h>

#include "DeliveryReportCb.h"

void kafka::DeliveryReportCb::dr_cb(RdKafka::Message &message)
{
    if (message.err())
    {
        std::cerr << "[KafkaProducer] Message delivery failed: " << message.errstr()
                  << std::endl;
        return;
    }

    std::clog << "[KafkaProducer] Delivered message to topic '" << message.topic_name()
              << "' at offset " << message.offset() << std::endl;
}
