#include "KafkaConsumer.h"

#include <memory>
#include <stdexcept>

#include "utils/log/singletonLogger.h"

namespace kafka
{
    namespace
    {
        void destroyConsumer(RdKafka::Consumer *consumer) noexcept
        {
            if (!consumer)
            {
                return;
            }

            consumer->poll(0);
            delete consumer;
        }

        void destroyTopic(RdKafka::Topic *topic) noexcept
        {
            delete topic;
        }

        void setConfigValue(RdKafka::Conf &conf, const std::string &key, const std::string &value)
        {
            std::string error;
            const auto result = conf.set(key, value, error);
            if (result != RdKafka::Conf::CONF_OK)
            {
                throw std::runtime_error("Failed to set '" + key + "': " + error);
            }
        }
    }

    KafkaConsumer::KafkaConsumer(const std::string &brokers, const std::string &topics)
        : consumer_{nullptr, destroyConsumer}, topic_{nullptr, destroyTopic}
    {
        std::string error;

        std::unique_ptr<RdKafka::Conf, std::function<void(RdKafka::Conf *)>> conf{
            RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL), [](RdKafka::Conf *ptr) { delete ptr; }};
        if (!conf)
        {
            throw std::runtime_error("Failed to create Kafka configuration");
        }

        setConfigValue(*conf, "metadata.broker.list", brokers);
        setConfigValue(*conf, "enable.auto.commit", "false");
        setConfigValue(*conf, "fetch.wait.max.ms", "0");

        RdKafka::Consumer *rawConsumer = RdKafka::Consumer::create(conf.get(), error);
        if (!rawConsumer)
        {
            throw std::runtime_error("Failed to create Kafka consumer: " + error);
        }
        consumer_.reset(rawConsumer);

        std::unique_ptr<RdKafka::Conf, std::function<void(RdKafka::Conf *)>> topicConf{
            RdKafka::Conf::create(RdKafka::Conf::CONF_TOPIC), [](RdKafka::Conf *ptr) { delete ptr; }};
        if (!topicConf)
        {
            throw std::runtime_error("Failed to allocate topic configuration");
        }

        RdKafka::Topic *rawTopic = RdKafka::Topic::create(consumer_.get(), topics, topicConf.get(), error);
        if (!rawTopic)
        {
            throw std::runtime_error("Failed to create Kafka topic: " + error);
        }
        topic_.reset(rawTopic);

        const auto startResult = consumer_->start(topic_.get(), 0, RdKafka::Topic::OFFSET_END);
        if (startResult != RdKafka::ERR_NO_ERROR)
        {
            throw std::runtime_error("Failed to start Kafka consumer: " + RdKafka::err2str(startResult));
        }

        utils::SingletonLogger::instance().logMeta(
            utils::SingletonLogger::MessageCode::INFO,
            "[KafkaConsumer] Connected to topic '" + topics + "'",
            __FILE__,
            __LINE__,
            __func__);
    }

    KafkaConsumer::~KafkaConsumer()
    {
        stopConsumeMessages();
    }

    void KafkaConsumer::ensureActive() const
    {
        if (!consumer_ || !topic_)
        {
            throw std::runtime_error("Kafka consumer is not initialised");
        }
    }

    void KafkaConsumer::consumeMessages(ExCosumeCb &ex_consume_cb, std::chrono::milliseconds timeout)
    {
        ensureActive();

        while (consume_.load(std::memory_order_acquire))
        {
            consumer_->consume_callback(topic_.get(), 0, static_cast<int>(timeout.count()), &ex_consume_cb, nullptr);
        }
    }

    std::string KafkaConsumer::consumeMessage(std::chrono::milliseconds timeout)
    {
        ensureActive();

        std::unique_ptr<RdKafka::Message> message{consumer_->consume(topic_.get(), 0, static_cast<int>(timeout.count()))};
        if (!message)
        {
            throw std::runtime_error("Failed to allocate Kafka message container");
        }

        switch (message->err())
        {
        case RdKafka::ERR_NO_ERROR:
            return std::string(static_cast<const char *>(message->payload()), message->len());

        case RdKafka::ERR__TIMED_OUT:
            return {};

        default:
            throw std::runtime_error("Kafka consume failed: " + message->errstr());
        }
    }

    void KafkaConsumer::stopConsumeMessages()
    {
        consume_.store(false, std::memory_order_release);

        if (consumer_ && topic_)
        {
            consumer_->stop(topic_.get(), 0);
            consumer_->poll(0);
        }

        topic_.reset();
        consumer_.reset();

        RdKafka::wait_destroyed(5000);
        utils::SingletonLogger::instance().logMeta(
            utils::SingletonLogger::MessageCode::INFO,
            "[KafkaConsumer] Shutdown complete",
            __FILE__,
            __LINE__,
            __func__);
}
}
