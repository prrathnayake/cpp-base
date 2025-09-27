#pragma once

#include <chrono>
#include <functional>
#include <map>
#include <mutex>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

namespace app_monitoring
{
    class MetricsRegistry
    {
    public:
        using Labels = std::map<std::string, std::string>;

        void registerCounter(const std::string &name, const std::string &help = "");
        void incrementCounter(const std::string &name, double value = 1.0);

        void registerGauge(const std::string &name, const std::string &help = "");
        void setGauge(const std::string &name, double value);

        void registerHistogram(const std::string &name, std::vector<double> buckets, const std::string &help = "");
        void observeHistogram(const std::string &name, double value);

        [[nodiscard]] std::string toPrometheus() const;

    private:
        struct Counter
        {
            double value{0.0};
            std::string help;
        };

        struct Gauge
        {
            double value{0.0};
            std::string help;
        };

        struct Histogram
        {
            std::vector<double> buckets;
            std::vector<std::uint64_t> counts;
            double sum{0.0};
            std::string help;
        };

        [[nodiscard]] static std::string sanitiseName(const std::string &name);
        void ensureCounterExists(const std::string &name);
        void ensureGaugeExists(const std::string &name);
        void ensureHistogramExists(const std::string &name, std::vector<double> buckets);

        std::unordered_map<std::string, Counter> counters_;
        std::unordered_map<std::string, Gauge> gauges_;
        std::unordered_map<std::string, Histogram> histograms_;

        mutable std::mutex mutex_;
    };

    class HealthCheckRegistry
    {
    public:
        using Check = std::function<bool(std::string &)>;

        void registerCheck(const std::string &name, Check check);
        [[nodiscard]] std::map<std::string, bool> runChecks() const;

    private:
        std::map<std::string, Check> checks_;
        mutable std::mutex mutex_;
    };

    class ScopedTimer
    {
    public:
        ScopedTimer(MetricsRegistry &registry, std::string histogram, double scale = 0.001);
        ~ScopedTimer();

    private:
        MetricsRegistry &registry_;
        std::string histogram_;
        double scale_;
        std::chrono::steady_clock::time_point start_;
    };
}

#include "MetricsRegistry.tcc"
