#include "KafkaProducer.h"

#include <utility>

#include "utils/log/singletonLogger.h"

namespace kafka
{
    namespace
    {
        void destroyProducer(RdKafka::Producer *producer) noexcept
        {
            if (!producer)
            {
                return;
            }

            producer->flush(10 * 1000);
            delete producer;
        }

        void destroyConfig(RdKafka::Conf *conf) noexcept
        {
            delete conf;
        }

        void setConfigValue(RdKafka::Conf &conf, const std::string &key, const std::string &value)
        {
            std::string error;
            const auto result = conf.set(key, value, error);
            if (result != RdKafka::Conf::CONF_OK)
            {
                throw KafkaError("Failed to set '" + key + "': " + error);
            }
        }
    }

    KafkaProducer::ConfPtr KafkaProducer::createConfig()
    {
        ConfPtr conf{RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL), destroyConfig};
        if (!conf)
        {
            throw KafkaError("Failed to allocate Kafka configuration");
        }
        return conf;
    }

    void KafkaProducer::configureCommonOptions(RdKafka::Conf &conf, const std::string &brokers)
    {
        setConfigValue(conf, "bootstrap.servers", brokers);
        setConfigValue(conf, "linger.ms", "0");
        setConfigValue(conf, "batch.num.messages", "30");
        setConfigValue(conf, "acks", "1");
        setConfigValue(conf, "compression.type", "lz4");

        std::string error;
        auto result = conf.set("dr_cb", &deliveryReportCallback_, error);
        if (result != RdKafka::Conf::CONF_OK)
        {
            throw KafkaError("Failed to attach delivery callback: " + error);
        }

        result = conf.set("delivery.report.only.error", "true", error);
        if (result != RdKafka::Conf::CONF_OK)
        {
            throw KafkaError("Failed to set delivery reporting: " + error);
        }
    }

    KafkaProducer::KafkaProducer(const std::string &brokers)
        : producer_{nullptr, destroyProducer}
    {
        auto conf = createConfig();
        configureCommonOptions(*conf, brokers);

        std::string error;
        RdKafka::Producer *rawProducer = RdKafka::Producer::create(conf.get(), error);
        if (!rawProducer)
        {
            throw KafkaError("Failed to create Kafka producer: " + error);
        }

        producer_.reset(rawProducer);
        utils::SingletonLogger::instance().logMeta(
            utils::SingletonLogger::MessageCode::INFO,
            "[KafkaProducer] Created producer for " + brokers,
            __FILE__,
            __LINE__,
            __func__);
    }

    KafkaProducer::~KafkaProducer()
    {
        try
        {
            flush();
        }
        catch (const std::exception &ex)
        {
            utils::SingletonLogger::instance().logMeta(
                utils::SingletonLogger::MessageCode::WARNING,
                std::string{"[KafkaProducer] Exception during flush: "} + ex.what(),
                __FILE__,
                __LINE__,
                __func__);
        }
    }

    void KafkaProducer::produceMessages(const std::string &topic, const std::string &message)
    {
        if (!producer_)
        {
            throw KafkaError("Kafka producer is not initialised");
        }

        constexpr std::size_t maxRetries = 3;
        for (std::size_t attempt = 0; attempt <= maxRetries; ++attempt)
        {
            auto error = producer_->produce(topic,
                                            RdKafka::Topic::PARTITION_UA,
                                            RdKafka::Producer::RK_MSG_COPY,
                                            const_cast<char *>(message.data()),
                                            message.size(),
                                            nullptr,
                                            0,
                                            0,
                                            nullptr,
                                            nullptr);

            if (error == RdKafka::ERR_NO_ERROR)
            {
                producer_->poll(0);
                return;
            }

            if (error == RdKafka::ERR__QUEUE_FULL && attempt < maxRetries)
            {
                producer_->poll(1000);
                continue;
            }

            throw KafkaError("Failed to produce to topic '" + topic + "': " + RdKafka::err2str(error));
        }
    }

    void KafkaProducer::flush(std::chrono::milliseconds timeout)
    {
        if (!producer_)
        {
            return;
        }

        const auto flushResult = producer_->flush(static_cast<int>(timeout.count()));
        if (flushResult != RdKafka::ERR_NO_ERROR)
        {
            utils::SingletonLogger::instance().logMeta(
                utils::SingletonLogger::MessageCode::WARNING,
                std::string{"[KafkaProducer] Flush finished with error: "} + RdKafka::err2str(flushResult),
                __FILE__,
                __LINE__,
                __func__);
        }

        const auto remaining = producer_->outq_len();
        if (remaining > 0)
        {
            utils::SingletonLogger::instance().logMeta(
                utils::SingletonLogger::MessageCode::WARNING,
                "[KafkaProducer] " + std::to_string(remaining) +
                    " message(s) remaining in queue after flush.",
                __FILE__,
                __LINE__,
                __func__);
        }
    }
}