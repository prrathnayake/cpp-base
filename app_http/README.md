# app_http

`app_http` contains HTTP client/server scaffolding for microservice integration. While the current utilities are lightweight, the module links against `app_utils` so future logging-enabled extensions are straightforward.

## Usage

```cmake
find_package(app_http REQUIRED)
target_link_libraries(your_target PRIVATE app_http)
```

Place additional adapters under `app_http/src/http` and rely on `utils::SingletonLogger` for request/response tracing to maintain consistency with the rest of the toolkit.
