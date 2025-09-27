# app_monitoring

`app_monitoring` provides lightweight observability building blocks—counters, gauges, histograms, health checks, and scoped timers—with built-in logging.

## Highlights

- Prometheus-compatible text exposition via `MetricsRegistry::toPrometheus`.
- Health check registry that logs outcomes and optional diagnostic messages.
- `ScopedTimer` helper to record operation durations in histograms automatically.

## Example

```cpp
#include "monitoring/MetricsRegistry.h"

app_monitoring::MetricsRegistry metrics;
metrics.registerCounter("orders_created", "Number of orders created");
metrics.incrementCounter("orders_created");

app_monitoring::ScopedTimer timer{metrics, "db_latency"};
// code to measure
```

Logs capture registrations, observations, and health-check results using `utils::SingletonLogger`.

## Build integration

```cmake
find_package(app_monitoring REQUIRED)
target_link_libraries(your_target PRIVATE app_monitoring)
```
