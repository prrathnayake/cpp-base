# app_utils

`app_utils` provides foundational utilities shared across the `cpp-base` ecosystem. It offers structured logging, chrono/date helpers, and JSON tooling that other modules depend on for consistent diagnostics and configuration handling.

## Highlights

- **Singleton logging** via `utils::SingletonLogger` for uniform file/console output with metadata.
- **JSON helpers** that wrap `nlohmann::json` for safe parsing, file IO, and typed value extraction.
- **Time utilities** for generating log-friendly timestamps and time conversions.

## Using the library

1. Add the package as a dependency in your CMake project:

   ```cmake
   find_package(app_utils REQUIRED)
   target_link_libraries(your_target PRIVATE app_utils)
   ```

2. Initialise and use the logger in your code:

   ```cpp
   #include "utils/log/singletonLogger.h"

   void doWork() {
       auto &logger = utils::SingletonLogger::instance();
       logger.logMeta(utils::SingletonLogger::MessageCode::INFO,
                      "Work scheduled",
                      __FILE__, __LINE__, __func__);
   }
   ```

3. Parse configuration safely:

   ```cpp
   #include "utils/json/json.h"

   auto json = utils::Json::readFromFile("config.json");
   auto port = utils::Json::getIntOrDefault(json, "port", 8080);
   ```

## Building from source

`app_utils` is part of the mono-repo build. To build it directly:

```bash
cmake -S app_utils/src -B build/app_utils
cmake --build build/app_utils
```

The module installs headers under `include/utils` and exports the `app_utils` target for downstream use.

## Integration tips

- Create the logger once per process; repeated calls to `instance()` reuse the same sink and log file.
- Use `logMeta` when context is important—it automatically records file, line, and function information.
- Configure the log file path via `SingletonLogger::instance("/path/to/log.txt")` during application start-up.
