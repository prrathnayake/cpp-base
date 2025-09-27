#pragma once

#include <atomic>
#include <chrono>
#include <functional>
#include <memory>
#include <string>

#include <librdkafka/rdkafkacpp.h>

#include "ConsumeCb.h"

namespace kafka
{
    class KafkaConsumer
    {
    public:
        KafkaConsumer(const std::string &brokers, const std::string &topics);
        ~KafkaConsumer();

        KafkaConsumer(const KafkaConsumer &) = delete;
        KafkaConsumer &operator=(const KafkaConsumer &) = delete;
        KafkaConsumer(KafkaConsumer &&) noexcept = default;
        KafkaConsumer &operator=(KafkaConsumer &&) noexcept = default;

        void consumeMessages(ExCosumeCb &ex_consume_cb, std::chrono::milliseconds timeout = std::chrono::seconds(1));
        std::string consumeMessage(std::chrono::milliseconds timeout = std::chrono::seconds(1));
        void stopConsumeMessages();

    private:
        using ConsumerPtr = std::unique_ptr<RdKafka::Consumer, std::function<void(RdKafka::Consumer *)>>;
        using TopicPtr = std::unique_ptr<RdKafka::Topic, std::function<void(RdKafka::Topic *)>>;

        void ensureActive() const;

        ConsumerPtr consumer_{nullptr, [](RdKafka::Consumer *) {}};
        TopicPtr topic_{nullptr, [](RdKafka::Topic *) {}};
        std::atomic<bool> consume_{true};
    };
}
