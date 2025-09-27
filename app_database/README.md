# app_database

`app_database` centralises PostgreSQL, MySQL, and SQLite helpers. Connection pools and query helpers now emit diagnostic output through `utils::SingletonLogger`, making it easier to trace SQL execution without touching `std::cout`/`std::cerr` directly.

## Modules

- `MySQLConnection` – wraps raw `MYSQL*` handles with RAII connection lifecycle management.
- `MySQLDatabase` – high-level CRUD helpers, SQL script execution, and row materialisation.
- (Additional drivers for PostgreSQL/SQLite are available in the source tree.)

## Logging behaviour

- Success paths (`initializeDatabase`, CRUD helpers, script execution) log at INFO level.
- Failures log at ERROR level with MySQL error messages and the offending statement for rapid diagnostics.

## Usage

```cmake
find_package(app_database REQUIRED)
target_link_libraries(your_target PRIVATE app_database)
```

```cpp
#include "database/MySQLDatabase.h"

int main() {
    database::MySQLDatabase db{"localhost", "user", "pass", 3306, ""};
    db.initializeDatabase("service_db");
    db.executeInsert("INSERT INTO audit(message) VALUES('started')");
}
```

## Notes

- Ensure the MySQL, PostgreSQL, and SQLite client libraries are installed on the build host.
- Link `app_utils` in your application (transitively provided by this module) if you need direct logger access.
- Prefer `logMeta` when extending the module so new diagnostics remain consistent.
