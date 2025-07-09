# 📦 cpp-base
# 🚀 C++ Backend Development Environment - Under Development

This repository provides a **fully configured Docker build environment** for C++ microservices development. including Kafka, RabbitMQ, gRPC, H3, OpenSSL, and more.

---

## 📦 Features

🚀 Features
- Kafka Integration: Utilizes librdkafka for high-performance message publishing and consumption.
- RabbitMQ Support: Employs rabbitmq-c and AMQP-CPP for message queuing.
- Geospatial Indexing: Incorporates Uber's H3 library for efficient spatial indexing.
- Utility Modules: Provides shared utilities for logging, configuration, and more.
- Conan Package Management: Manages dependencies and packaging using Conan.

📚 Pre-requisite Libraries
+ [rabbitmq-c](https://github.com/alanxz/rabbitmq-c.git)
+ [amqpcpp](https://github.com/akalend/amqpcpp.git)
+ [AMQP-CPP](https://github.com/CopernicaMarketingSoftware/AMQP-CPP.git)
+ [librdkafka](https://github.com/confluentinc/librdkafka.git)
+ [uber H3](https://github.com/uber/h3.git)
+ [OpenSSL](https://github.com/openssl/openssl.git)
+ [PostgreSQL](https://www.postgresql.org/download/)
+ [MySQL Client Library](https://dev.mysql.com/downloads/connector/c/)
+ [SQLite](https://www.sqlite.org/download.html)

---

## 📁 Project Structure

```
bash
cpp-base/
├── app_algorithms/       # Algorithmic utilities
├── app_h3/               # Geospatial indexing with H3
├── app_kafka/            # Kafka integration
├── app_rabbitMQ/         # RabbitMQ integration
├── app_utils/            # Shared utilities
├── CMakeLists.txt        # Build configuration
├── conanfile.py          # Conan package definitions
└── readme.md             # Project documentation
```
