# app_kafka

`app_kafka` wraps `librdkafka` with higher-level producer and consumer helpers that are pre-integrated with the shared `SingletonLogger`. The module makes it easy to publish and consume messages with consistent retry, delivery reporting, and structured diagnostics.

## Features

- `KafkaProducer` with configurable connection settings and built-in delivery callbacks.
- `KafkaConsumer` that manages topic subscriptions and exposes both callback-driven and synchronous consumption patterns.
- Logging of connection lifecycle, delivery successes/failures, and consumption errors through `utils::SingletonLogger`.

## Usage

1. Link the library:

   ```cmake
   find_package(app_kafka REQUIRED)
   target_link_libraries(your_target PRIVATE app_kafka)
   ```

2. Produce a message:

   ```cpp
   #include "kafka/KafkaProducer.h"

   kafka::KafkaProducer producer{"localhost:9092"};
   producer.produceMessages("events", "payload");
   producer.flush();
   ```

3. Consume messages with logging already enabled:

   ```cpp
   #include "kafka/KafkaConsumer.h"
   #include "kafka/ConsumeCb.h"

   kafka::KafkaConsumer consumer{"localhost:9092", "events"};
   kafka::ExCosumeCb callback;
   consumer.consumeMessages(callback, std::chrono::milliseconds{500});
   ```

## Building

`librdkafka` must be available. Inside this repository run:

```bash
cmake -S . -B build
cmake --build build --target app_kafka
```

If the dependency is missing the module falls back to a header-only stub with `APP_KAFKA_MISSING_DEPENDENCIES` defined.

## Integration tips

- Call `utils::SingletonLogger::instance()` early in your application to customise the log sink.
- Handle exceptions thrown by the producer/consumer constructors—they propagate configuration and connection issues.
- Use the provided callbacks (`DeliveryReportCb`, `ExCosumeCb`) as baselines and extend them to add domain-specific logic while keeping logging centralised.
