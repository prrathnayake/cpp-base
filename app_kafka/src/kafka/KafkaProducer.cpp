#include <iostream>
#include <string>
#include <librdkafka/rdkafkacpp.h>

#include "DeliveryReportCb.h"
#include "KafkaProducer.h"

kafka::KafkaProducer::KafkaProducer(std::string brokers)
{
    RdKafka::Conf *conf = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);

    std::string errstr;

    if (conf->set("bootstrap.servers", brokers, errstr) !=
        RdKafka::Conf::CONF_OK)
    {
        std::cerr << errstr << std::endl;
        exit(1);
    }

    if (conf->set("linger.ms", "0", errstr) !=
        RdKafka::Conf::CONF_OK)
    {
        std::cerr << errstr << std::endl;
        exit(1);
    }

    if (conf->set("batch.num.messages", "30", errstr) !=
        RdKafka::Conf::CONF_OK)
    {
        std::cerr << errstr << std::endl;
        exit(1);
    }

    if (conf->set("acks", "1", errstr) !=
        RdKafka::Conf::CONF_OK)
    {
        std::cerr << errstr << std::endl;
        exit(1);
    }

    if (conf->set("compression.type", "lz4", errstr) !=
        RdKafka::Conf::CONF_OK)
    {
        std::cerr << errstr << std::endl;
        exit(1);
    }

    DeliveryReportCb ex_dr_cb;

    if (conf->set("dr_cb", &ex_dr_cb, errstr) != RdKafka::Conf::CONF_OK)
    {
        std::cerr << errstr << std::endl;
        exit(1);
    }

    if (conf->set("delivery.report.only.error", "true", errstr) !=
        RdKafka::Conf::CONF_OK)
    {
        std::cerr << errstr << std::endl;
        exit(1);
    }

    producer = RdKafka::Producer::create(conf, errstr);
    if (!producer)
    {
        std::cerr << "Failed to create producer: " << errstr << std::endl;
        exit(1);
    }

    delete conf;
    std::cout << "Creating Kafka producer" << std::endl;
}

void kafka::KafkaProducer::produceMessages(std::string topic, std::string message)
{
retry:
    RdKafka::ErrorCode err = producer->produce(
        topic,
        RdKafka::Topic::PARTITION_UA,
        RdKafka::Producer::RK_MSG_COPY,
        const_cast<char *>(message.c_str()), message.size(),
        NULL, 0,
        0,
        NULL,
        NULL);

    if (err != RdKafka::ERR_NO_ERROR)
    {
        std::cerr << "% Failed to produce to topic " << topic << ": "
                  << RdKafka::err2str(err) << std::endl;

        if (err == RdKafka::ERR__QUEUE_FULL)
        {
            producer->poll(1000);
            goto retry;
        }
    }
}

kafka::KafkaProducer::~KafkaProducer()
{
    std::cerr << "% Flushing final messages..." << std::endl;
    producer->flush(10 * 1000);
    if (producer->outq_len() > 0)
        std::cerr << producer->outq_len()
                  << " message(s) were not delivered" << std::endl;
    delete producer;
    std::cout << "Stopping Kafka producer" << std::endl;
}