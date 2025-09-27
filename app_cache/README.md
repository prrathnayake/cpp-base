# app_cache

`app_cache` supplies an in-memory cache abstraction with TTL support and hash operations. While designed as a stand-in for Redis/Memcached integrations, it wires all operations through `utils::SingletonLogger` for consistent diagnostics.

## Features

- `set`/`get` with optional TTL and automatic expiry purging.
- Hash operations (`hset`, `hget`, `hgetall`) for session or document-style data.
- Global flush for integration tests and local development.

## Usage

```cpp
#include "cache/CacheClient.h"

app_cache::CacheClient cache;
cache.connect("default", "memory://");
cache.set("key", "value", std::chrono::seconds{30});
if (auto value = cache.get("key")) {
    // use value
}
```

Each operation logs whether it hit, missed, or expired entries, supporting easy troubleshooting when wiring the cache into other services.

## CMake

```cmake
find_package(app_cache REQUIRED)
target_link_libraries(your_target PRIVATE app_cache)
```
