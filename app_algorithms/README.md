# app_algorithms

`app_algorithms` bundles reusable data-structure and algorithm snippets shared across services. While most helpers are header-only, the library links against `app_utils` so utilities such as `SingletonLogger` are readily available when extending the module.

## Usage

```cmake
find_package(app_algorithms REQUIRED)
target_link_libraries(your_target PRIVATE app_algorithms)
```

Headers are installed under `include/algorithms`. Add new helpers alongside existing namespaces to keep the surface area coherent.

## Extending

- Prefer header-only utilities when performance is critical.
- Use `utils::SingletonLogger` for any runtime diagnostics; the dependency is already wired in via CMake.
- Document algorithm complexity in the header comments so future consumers can make informed decisions.
