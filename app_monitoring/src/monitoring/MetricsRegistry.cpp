#include "MetricsRegistry.h"

#include <cctype>
#include <iomanip>
#include <limits>
#include <sstream>

#include "utils/log/singletonLogger.h"

namespace app_monitoring
{
    namespace
    {
        std::string formatPrometheusHelp(const std::string &name, const std::string &help)
        {
            if (help.empty())
            {
                return {};
            }

            std::ostringstream stream;
            stream << "# HELP " << name << ' ' << help << '\n';
            return stream.str();
        }

        std::string formatPrometheusType(const std::string &name, const std::string &type)
        {
            std::ostringstream stream;
            stream << "# TYPE " << name << ' ' << type << '\n';
            return stream.str();
        }
    }

    void MetricsRegistry::registerCounter(const std::string &name, const std::string &help)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        auto &counter = counters_[name];
        counter.help = help;
        utils::SingletonLogger::instance().logMeta(
            utils::SingletonLogger::MessageCode::DEBUG,
            "Registered counter: " + name,
            __FILE__,
            __LINE__,
            __func__);
    }

    void MetricsRegistry::incrementCounter(const std::string &name, double value)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        ensureCounterExists(name);
        counters_[name].value += value;
        utils::SingletonLogger::instance().logMeta(
            utils::SingletonLogger::MessageCode::INFO,
            "Counter '" + name + "' incremented by " + std::to_string(value),
            __FILE__,
            __LINE__,
            __func__);
    }

    void MetricsRegistry::registerGauge(const std::string &name, const std::string &help)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        auto &gauge = gauges_[name];
        gauge.help = help;
        utils::SingletonLogger::instance().logMeta(
            utils::SingletonLogger::MessageCode::DEBUG,
            "Registered gauge: " + name,
            __FILE__,
            __LINE__,
            __func__);
    }

    void MetricsRegistry::setGauge(const std::string &name, double value)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        ensureGaugeExists(name);
        gauges_[name].value = value;
        utils::SingletonLogger::instance().logMeta(
            utils::SingletonLogger::MessageCode::INFO,
            "Gauge '" + name + "' set to " + std::to_string(value),
            __FILE__,
            __LINE__,
            __func__);
    }

    void MetricsRegistry::registerHistogram(const std::string &name, std::vector<double> buckets, const std::string &help)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        ensureHistogramExists(name, std::move(buckets));
        histograms_[name].help = help;
        utils::SingletonLogger::instance().logMeta(
            utils::SingletonLogger::MessageCode::DEBUG,
            "Registered histogram: " + name,
            __FILE__,
            __LINE__,
            __func__);
    }

    void MetricsRegistry::observeHistogram(const std::string &name, double value)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        ensureHistogramExists(name, {0.1, 0.5, 1.0, 5.0});
        auto &histogram = histograms_[name];
        histogram.sum += value;
        for (std::size_t i = 0; i < histogram.buckets.size(); ++i)
        {
            if (value <= histogram.buckets[i])
            {
                histogram.counts[i] += 1;
            }
        }
        histogram.counts.back() += 1; // +Inf bucket
        utils::SingletonLogger::instance().logMeta(
            utils::SingletonLogger::MessageCode::INFO,
            "Observed histogram '" + name + "' with value " + std::to_string(value),
            __FILE__,
            __LINE__,
            __func__);
    }

    std::string MetricsRegistry::toPrometheus() const
    {
        std::lock_guard<std::mutex> lock(mutex_);
        std::ostringstream stream;

        for (const auto &[name, counter] : counters_)
        {
            const auto metricName = sanitiseName(name);
            stream << formatPrometheusHelp(metricName, counter.help)
                   << formatPrometheusType(metricName, "counter")
                   << metricName << " " << counter.value << "\n";
        }

        for (const auto &[name, gauge] : gauges_)
        {
            const auto metricName = sanitiseName(name);
            stream << formatPrometheusHelp(metricName, gauge.help)
                   << formatPrometheusType(metricName, "gauge")
                   << metricName << " " << gauge.value << "\n";
        }

        for (const auto &[name, histogram] : histograms_)
        {
            const auto metricName = sanitiseName(name);
            stream << formatPrometheusHelp(metricName, histogram.help)
                   << formatPrometheusType(metricName, "histogram");

            double cumulative = 0.0;
            for (std::size_t i = 0; i < histogram.buckets.size(); ++i)
            {
                cumulative += histogram.counts[i];
                stream << metricName << "_bucket{le=\"" << histogram.buckets[i] << "\"} " << cumulative << "\n";
            }
            cumulative += histogram.counts.back();
            stream << metricName << "_bucket{le=\"+Inf\"} " << cumulative << "\n";
            stream << metricName << "_sum " << histogram.sum << "\n";
            stream << metricName << "_count " << cumulative << "\n";
        }

        return stream.str();
    }

    std::string MetricsRegistry::sanitiseName(const std::string &name)
    {
        std::string result = name;
        for (char &ch : result)
        {
            if (!(std::isalnum(static_cast<unsigned char>(ch)) || ch == '_'))
            {
                ch = '_';
            }
        }
        return result;
    }

    void MetricsRegistry::ensureCounterExists(const std::string &name)
    {
        if (!counters_.count(name))
        {
            counters_[name] = Counter{};
        }
    }

    void MetricsRegistry::ensureGaugeExists(const std::string &name)
    {
        if (!gauges_.count(name))
        {
            gauges_[name] = Gauge{};
        }
    }

    void MetricsRegistry::ensureHistogramExists(const std::string &name, std::vector<double> buckets)
    {
        if (!histograms_.count(name))
        {
            if (buckets.empty())
            {
                buckets = {0.1, 0.5, 1.0, 5.0};
            }
            buckets.push_back(std::numeric_limits<double>::infinity());
            Histogram histogram;
            histogram.buckets = buckets;
            histogram.counts.resize(histogram.buckets.size(), 0);
            histograms_[name] = std::move(histogram);
        }
    }

    void HealthCheckRegistry::registerCheck(const std::string &name, Check check)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        checks_[name] = std::move(check);
        utils::SingletonLogger::instance().logMeta(
            utils::SingletonLogger::MessageCode::INFO,
            "Registered health check: " + name,
            __FILE__,
            __LINE__,
            __func__);
    }

    std::map<std::string, bool> HealthCheckRegistry::runChecks() const
    {
        std::lock_guard<std::mutex> lock(mutex_);
        std::map<std::string, bool> results;
        for (const auto &[name, check] : checks_)
        {
            std::string message;
            const bool status = check ? check(message) : false;
            results[name] = status;
            auto level = status ? utils::SingletonLogger::MessageCode::INFO : utils::SingletonLogger::MessageCode::WARNING;
            utils::SingletonLogger::instance().logMeta(
                level,
                "Health check '" + name + "' => " + (status ? "healthy" : "unhealthy") + (message.empty() ? std::string{} : ": " + message),
                __FILE__,
                __LINE__,
                __func__);
        }
        return results;
    }

    ScopedTimer::ScopedTimer(MetricsRegistry &registry, std::string histogram, double scale)
        : registry_(registry), histogram_(std::move(histogram)), scale_(scale), start_(std::chrono::steady_clock::now())
    {
    }

    ScopedTimer::~ScopedTimer()
    {
        const auto end = std::chrono::steady_clock::now();
        const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start_).count() * scale_;
        registry_.observeHistogram(histogram_, static_cast<double>(duration));
    }
}
