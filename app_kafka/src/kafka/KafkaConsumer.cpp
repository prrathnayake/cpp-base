#include <iostream>
#include <string>
#include <memory>

#include <librdkafka/rdkafkacpp.h>

#include "ConsumeCb.h"
#include "KafkaConsumer.h"

namespace kafka {

KafkaConsumer::KafkaConsumer(std::string brokers, std::string topics) {
    std::string errstr;

    // Global config
    RdKafka::Conf *conf = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);
    if (conf->set("metadata.broker.list", brokers, errstr) != RdKafka::Conf::CONF_OK) {
        std::cerr << "Broker config error: " << errstr << std::endl;
        exit(1);
    }

    if (conf->set("enable.auto.commit", "false", errstr) != RdKafka::Conf::CONF_OK) {
        std::cerr << "Auto commit config error: " << errstr << std::endl;
        exit(1);
    }

    if (conf->set("fetch.wait.max.ms", "0", errstr) != RdKafka::Conf::CONF_OK) {
        std::cerr << "Fetch wait config error: " << errstr << std::endl;
        exit(1);
    }

    consumer = RdKafka::Consumer::create(conf, errstr);
    delete conf;

    if (!consumer) {
        std::cerr << "Failed to create Kafka consumer: " << errstr << std::endl;
        exit(1);
    }

    // Topic config
    RdKafka::Conf *tconf = RdKafka::Conf::create(RdKafka::Conf::CONF_TOPIC);
    C_topic = RdKafka::Topic::create(consumer, topics, tconf, errstr);
    delete tconf;

    if (!C_topic) {
        std::cerr << "Failed to create Kafka topic: " << errstr << std::endl;
        exit(1);
    }

    // Start consumer from beginning (or change to OFFSET_END if preferred)
    RdKafka::ErrorCode resp = consumer->start(C_topic, 0, RdKafka::Topic::OFFSET_END);
    if (resp != RdKafka::ERR_NO_ERROR) {
        std::cerr << "Consumer start failed: " << RdKafka::err2str(resp) << std::endl;
        exit(1);
    }

    std::cout << "Kafka consumer created and started successfully.\n";
}

void KafkaConsumer::consumeMessages(ExCosumeCb ex_consume_cb) {
    std::cout << "Consuming messages (callback mode)...\n";
    int use_ccb = 1;

    while (consume) {
        consumer->consume_callback(C_topic, 0, 1000, &ex_consume_cb, &use_ccb);
    }

    consumer->stop(C_topic, 0);
    RdKafka::wait_destroyed(5000);
}

std::string KafkaConsumer::consumeMessage() {
    std::unique_ptr<RdKafka::Message> msg(consumer->consume(C_topic, 0, -1));
    std::string payload;

    switch (msg->err()) {
        case RdKafka::ERR_NO_ERROR:
            payload = std::string(static_cast<const char *>(msg->payload()), msg->len());
            break;

        case RdKafka::ERR__TIMED_OUT:
            std::cerr << "Kafka consume timed out.\n";
            break;

        default:
            std::cerr << "Kafka consume failed: " << msg->errstr() << std::endl;
            break;
    }

    return payload;
}

void KafkaConsumer::stopConsumeMessages() {
    consume = false;
     if (consumer && C_topic) {
        consumer->stop(C_topic, 0);
    }

    delete C_topic;
    delete consumer;

    std::cout << "Kafka consumer destroyed.\n";
}

KafkaConsumer::~KafkaConsumer() {
    if (consumer && C_topic) {
        consumer->stop(C_topic, 0);
    }

    delete C_topic;
    delete consumer;

    std::cout << "Kafka consumer destroyed.\n";
}

}  // namespace kafka
