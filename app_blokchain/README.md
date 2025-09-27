# app_blockchain

`app_blockchain` is a teaching-oriented blockchain prototype. Logging has been migrated to `utils::SingletonLogger`, ensuring mining events, signature operations, and chain inspection all emit consistent diagnostics.

## Components

- Mining loop (`node/miner.cpp`) with SHA-256 hashing and nonce management.
- RSA utilities for transaction signing and verification.
- `Blockchain::printBlockchain` to inspect the in-memory chain with INFO-level logging.

## Usage

```cmake
find_package(app_blockchain REQUIRED)
target_link_libraries(your_target PRIVATE app_blockchain)
```

```cpp
#include "blockchain/blockchain.h"

int main() {
    blockchain::Blockchain chain;
    chain.printBlockchain();
}
```

The logger will emit each block with associated metadata, making the console output friendlier for demos and integration tests.

## Dependencies

- OpenSSL for hashing and RSA operations.
- `app_utils` for logging support (linked transitively).

Ensure OpenSSL development headers are available before building the module.
