#Distributed Key-Value Store (C++, gRPC, Bazel)

A distributed, sharded key-value store built using the modern Google infrastructure stack.

## Architecture
* **Communication:** gRPC with Protocol Buffers (Synchronous API).
* **Build System:** Bazel (Hermetic build with Bzlmod).
* **Concurrency:** Thread-safe in-memory storage using `absl::Mutex` and `absl::flat_hash_map`.
* **Distribution:** Client-side sharding using consistent hashing strategies.

## Project Structure
* `server.cc`: Implements the `Put`/`Get` logic with thread safety annotations.
* `client.cc`: A smart client that routes requests to specific shards based on key hashing.
* `kv_store.proto`: Service definition.

## Getting Started

### Prerequisites
* Bazelisk (Bazel 7.0+)
* C++ Compiler (GCC/Clang) supporting C++17

### Running the Distributed System
1.  **Start Shard 0 (Port 50051):**
    ```bash
    bazel run //kvstore:server -- 50051
    ```
2.  **Start Shard 1 (Port 50052):**
    ```bash
    bazel run //kvstore:server -- 50052
    ```
3.  **Run the Client:**
    ```bash
    bazel run //kvstore:client
    ```

## Testing
Unit tests are implemented using **GoogleTest** and isolate logic from networking.
```bash
bazel test //kvstore:kv_store_test