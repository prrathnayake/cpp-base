# app_rabbitMQ

`app_rabbitMQ` provides a lightweight consumer wrapper around `AMQP-CPP`/`rabbitmq-c`, forwarding all message handling events to the shared `SingletonLogger`. It demonstrates how to bootstrap queue consumption while keeping logging consistent across services.

## Capabilities

- Automatic queue declaration and polling loop management via `RabbitMQconsumer`.
- Hook point (`onMessage`) for custom message processing with pre-wired logging.
- Graceful stop via atomic flags to integrate cleanly with service shutdown sequences.

## Getting started

1. Link the library:

   ```cmake
   find_package(app_rabbitmq REQUIRED)
   target_link_libraries(your_target PRIVATE app_rabbitmq)
   ```

2. Implement your consumer:

   ```cpp
   #include "rabbitMQ/consumer.h"

   class AuditConsumer : public rabbitMQ::RabbitMQconsumer {
   public:
       void onMessage(const std::string &payload) override {
           // Business logic here; logging handled by base implementation.
       }
   };
   ```

3. Start consuming:

   ```cpp
   AuditConsumer consumer;
   consumer.consumeMessages("amqp://user:pass@localhost/", "events.queue");
   ```

## Logging integration

`onMessage` already emits INFO-level logs with the message payload. Override the method to extend behaviour while optionally chaining to the base implementation for audit trails.

## Build notes

If the AMQP libraries are unavailable the module builds as a header-only stub with `APP_RABBITMQ_MISSING_DEPENDENCIES` defined, allowing downstream code to compile while skipping runtime operations.
