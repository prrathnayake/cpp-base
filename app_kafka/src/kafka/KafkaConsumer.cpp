#include <iostream>
#include <string>
#include <librdkafka/rdkafkacpp.h>

#include "ConsumeCb.h"
#include "KafkaConsumer.h"

kafka::KafkaConsumer::KafkaConsumer(std::string brokers, std::string topics)
{
    std::string errstr;
    RdKafka::Conf *conf = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);

    if (conf->set("metadata.broker.list", brokers, errstr) !=
        RdKafka::Conf::CONF_OK)
    {
        std::cerr << errstr << std::endl;
        exit(1);
    }
    if (conf->set("enable.auto.commit", "false", errstr) !=
        RdKafka::Conf::CONF_OK)
    {
        std::cerr << errstr << std::endl;
        exit(1);
    }

    if (conf->set("fetch.wait.max.ms", "0", errstr) !=
        RdKafka::Conf::CONF_OK)
    {
        std::cerr << errstr << std::endl;
        exit(1);
    }

    consumer = RdKafka::Consumer::create(conf, errstr);
    if (!consumer)
    {
        std::cerr << "Failed to create consumer: " << errstr << std::endl;
        exit(1);
    }

    delete conf;

    RdKafka::Conf *tconf = RdKafka::Conf::create(RdKafka::Conf::CONF_TOPIC);

    C_topic = RdKafka::Topic::create(consumer, topics, tconf, errstr);

    if (!C_topic)
    {
        std::cerr << "Failed to create topic: " << errstr << std::endl;
        exit(1);
    }

    delete tconf;

    RdKafka::ErrorCode resp = consumer->start(C_topic, 0, RdKafka::Topic::OFFSET_END);

    if (resp != RdKafka::ERR_NO_ERROR)
    {
        std::cerr << "Failed to create consumer: " << errstr << std::endl;
        exit(1);
    }

    std::cout << "Creating Kafka consumer" << std::endl;
}

void kafka::KafkaConsumer::consumeMessages(ExCosumeCb ex_consume_cb)
{
    std::cout << "Starting Kafka consumer" << std::endl;
    int use_ccb = 1;
    while (consume)
    {
        consumer->consume_callback(C_topic, 0, 0, &ex_consume_cb, &use_ccb);
    }
    consumer->stop(C_topic, 0);
    RdKafka::wait_destroyed(5000);
}

void kafka::KafkaConsumer::stopConsumeMessages()
{
    consume = false;
    std::cout << "Stopping Kafka consumer" << std::endl;
}

kafka::KafkaConsumer::~KafkaConsumer()
{
    delete consumer;
}
