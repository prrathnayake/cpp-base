# app_config

`app_config` standardises configuration loading for services built on `cpp-base`. It merges default, environment-specific, and runtime JSON documents while resolving environment-variable placeholders and emitting structured logs.

## Highlights

- Layered merge order: defaults → environment → runtime overrides.
- `${VAR}` placeholder substitution via a pluggable resolver (defaults to `std::getenv`).
- Thread-safe access with dotted-key lookups and typed getters.

## Example

```cpp
#include "config/ConfigurationManager.h"

app_config::ConfigurationManager config{"payments"};
config.loadDefaults("config/defaults.json");
config.loadEnvironment("config/prod.json");
config.applyRuntimeOverrides({{"http", {{"port", 8081}}}});

int port = config.get<int>("http.port", 8080);
```

Log entries record each layer load as well as merge operations, making troubleshooting straightforward.

## CMake integration

```cmake
find_package(app_config REQUIRED)
target_link_libraries(your_target PRIVATE app_config)
```

The target exports headers under `include/config` so other repositories can reuse the API directly.
