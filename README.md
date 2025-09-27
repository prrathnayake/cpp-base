# cpp-base

## Overview

`cpp-base` is a modular C++ backend workspace that centralises the build recipes for a collection of microservice-oriented
libraries. The repository is designed to be used together with the
[`ccp-tools`](https://github.com/) environment, which provides reproducible Docker images and helper
scripts for building the toolchain and third-party dependencies. The
project demonstrates messaging (Kafka, RabbitMQ), geospatial indexing (H3),
and general-purpose utilities that can be reused across microservices.

## Getting Started

### Prerequisites

- Docker and Docker Compose (when using the `ccp-tools` development containers).
- [Conan 2](https://docs.conan.io/2) package manager.
- CMake 3.21 or newer.
- A C++20-compatible compiler (GCC 11+, Clang 12+, or MSVC 2019 16.11+).
- Git submodules pulled for any modules you keep in separate repositories.

Optional integrations such as Kafka, RabbitMQ, PostgreSQL, MySQL, and
OpenSSL require their respective development headers and runtime libraries
when building outside the pre-configured containers.

### Clone the repository

```bash
git clone https://github.com/<your-account>/cpp-base.git
cd cpp-base
```

### Using `ccp-tools`

The recommended way to work with the repository is via the dedicated
`ccp-tools` workspace. It supplies a container image with the required
compilers and dependency mirrors, and exposes helper scripts for starting
interactive shells inside that environment. Refer to the `ccp-tools`
documentation for detailed setup instructions.

### Configure dependencies with Conan

Install the remotes you need and authenticate with your Conan server if
applicable. Once you are inside the container (or on a host with the
necessary tooling), install the dependencies:

```bash
conan install . --output-folder=build --build=missing
```

The command generates the CMake toolchain files and pulls any required
third-party packages that are not already available in your remotes.

### Build the project

After dependency resolution you can configure and build all modules via
Conan:

```bash
conan build . --output-folder=build --build=missing
```

The build artefacts and generated files will be placed under the `build/`
directory. You can customise the build type and other settings via the
standard Conan options, for example `-s build_type=Release`.

### Running unit tests

Each application module may expose its own test targets. Once the build
completes you can execute CTest from the generated build directory:

```bash
cd build
ctest --output-on-failure
```

## Repository layout

```
cpp-base/
├── app_algorithms/   # Algorithmic utilities shared across services
├── app_database/     # Database client helpers (PostgreSQL, MySQL, SQLite)
├── app_h3/           # Geospatial indexing helpers powered by Uber's H3
├── app_http/         # HTTP-related abstractions and clients
├── app_kafka/        # Kafka producers/consumers built on librdkafka
├── app_rabbitMQ/     # RabbitMQ connectors using rabbitmq-c and AMQP-CPP
├── app_utils/        # Shared logging, configuration, and system helpers
├── CMakeLists.txt    # Top-level CMake entry point
├── conanfile.py      # Conan recipe orchestrating sub-packages
└── README.md         # Project documentation
```

## Development guidelines

- Follow the existing directory structure when adding new modules.
- Keep public headers minimal and document any exported APIs.
- Prefer Conan packages for external dependencies so builds remain
  reproducible across environments.
- Remember to update this README and the NOTICE file when new third-party
  dependencies are introduced.

## Support and contributions

Issues and contributions are welcome. Please open a GitHub issue to report
bugs or discuss feature requests. When submitting pull requests, ensure the
CI pipeline (if configured) passes and add tests for new behaviour.

## License

This project is distributed under the MIT License. See [LICENSE](LICENSE)
for details.
