#include <iostream>
#include <string>
#include <librdkafka/rdkafkacpp.h>

#include "DeliveryReportCb.h"

void kafka::DeliveryReportCb::dr_cb(RdKafka::Message &message)
{
    if (message.err())
        std::cerr << "% Message delivery failed: " << message.errstr()
                  << std::endl;
}
