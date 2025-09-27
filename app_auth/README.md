# app_auth

`app_auth` offers JWT issuance and verification helpers powered by OpenSSL and the shared logger. It standardises token creation for microservices that need stateless authentication.

## Capabilities

- HS256 signing with constant-time verification.
- Automatic `iss`, `aud`, `iat`, and `exp` claim management.
- Detailed INFO/WARNING/ERROR logs for token issuance, expiration, and signature failures.

## Usage

```cpp
#include "auth/JwtManager.h"

app_auth::JwtManager jwt{"supersecret", "cpp-base", "clients"};
auto token = jwt.issueToken({{"sub", "user-123"}}, std::chrono::hours{1});

nlohmann::json payload;
auto result = jwt.verifyToken(token, payload);
if (!result.success) {
    // inspect result.message
}
```

## CMake

```cmake
find_package(app_auth REQUIRED)
target_link_libraries(your_target PRIVATE app_auth)
```

Ensure OpenSSL is available in your build environment; the module links against `OpenSSL::Crypto`.
