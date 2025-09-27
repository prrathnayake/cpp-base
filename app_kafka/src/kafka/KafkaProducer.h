#pragma once

#include <chrono>
#include <functional>
#include <memory>
#include <stdexcept>
#include <string>

#include <librdkafka/rdkafkacpp.h>

#include "DeliveryReportCb.h"

namespace kafka
{
    class KafkaError : public std::runtime_error
    {
    public:
        explicit KafkaError(const std::string &message) : std::runtime_error(message) {}
    };

    class KafkaProducer
    {
    public:
        explicit KafkaProducer(const std::string &brokers);
        ~KafkaProducer();

        KafkaProducer(const KafkaProducer &) = delete;
        KafkaProducer &operator=(const KafkaProducer &) = delete;
        KafkaProducer(KafkaProducer &&) noexcept = default;
        KafkaProducer &operator=(KafkaProducer &&) noexcept = default;

        void produceMessages(const std::string &topic, const std::string &message);
        void flush(std::chrono::milliseconds timeout = std::chrono::seconds(10));

    private:
        using ProducerPtr = std::unique_ptr<RdKafka::Producer, std::function<void(RdKafka::Producer *)>>;
        using ConfPtr = std::unique_ptr<RdKafka::Conf, std::function<void(RdKafka::Conf *)>>;

        static ConfPtr createConfig();
        void configureCommonOptions(RdKafka::Conf &conf, const std::string &brokers);

        DeliveryReportCb deliveryReportCallback_;
        ProducerPtr producer_{nullptr, [](RdKafka::Producer *) {}};
    };
}
