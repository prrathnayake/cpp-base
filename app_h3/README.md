# app_h3

`app_h3` exposes a focused wrapper around Uber's H3 geospatial indexing library with unified error logging. All conversion helpers now use `utils::SingletonLogger` to report failures.

## Key APIs

- `H3::getH3Index` – convert lat/long to an H3 index.
- `H3::getBoundaries` – retrieve cell boundaries for visualisation.
- `H3::getNeighbors` – fetch neighbouring cells with automatic error logging.

## Usage

```cmake
find_package(app_h3 REQUIRED)
target_link_libraries(your_target PRIVATE app_h3)
```

```cpp
#include "h3/h3.h"

auto index = h3::H3::getH3Index(51.5, -0.1, 9);
```

If H3 returns an error the logger prints an ERROR-level entry with the function metadata, allowing quick diagnosis without additional instrumentation.
