#include <iostream>
#include <memory>
#include <string>

#include "kvstore/kv_store.grpc.pb.h"
#include <grpcpp/grpcpp.h>

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using kvstore::GetRequest;
using kvstore::GetResponse;
using kvstore::KeyValueService;
using kvstore::PutRequest;
using kvstore::PutResponse;

class ShardedClient {
public:
  // Constructor
  ShardedClient(const std::vector<std::string> &server_ports) {
    for (const auto &port : server_ports) { // auto& auto detects type
      std::string target = "localhost:" + port;
      auto channel =
          grpc::CreateChannel(target, grpc::InsecureChannelCredentials());
      stubs_.push_back(KeyValueService::NewStub(channel));
      std::cout << "Connected to shard on " << target << std::endl;
    }
  };

  // helper
  int GetShardIndex(const std::string &key) {
    // convrts key to very big #
    size_t hash_val = std::hash<std::string>{}(key);
    return hash_val % stubs_.size();
  }

  void Put(const std::string &key, const std::string &value) {
    int shard_idx = GetShardIndex(key);

    PutRequest request;
    request.set_key(key);
    request.set_value(value);

    PutResponse reply;
    ClientContext context;

    // The actual network call
    Status status = stubs_[shard_idx]->Put(&context, request, &reply);

    if (status.ok()) {
      std::cout << "[Shard " << shard_idx << "] Put Success: " << key
                << std::endl;
    } else {
      std::cout << "[Shard " << shard_idx << "] Put Failed: " << std::endl;
    }
  }

  void Get(const std::string &key) {
    int shard_idx = GetShardIndex(key);
    GetRequest request;
    request.set_key(key);

    GetResponse reply;
    ClientContext context;

    // The actual network call
    Status status = stubs_[shard_idx]->Get(&context, request, &reply);

    if (status.ok() && reply.found()) {
      std::cout << "[Shard " << shard_idx << "] Get Success: " << key
                << std::endl;
    } else {
      std::cout << "[Shard " << shard_idx << "] Key not found." << std::endl;
    }
  }

private:
  std::vector<std::unique_ptr<KeyValueService::Stub>> stubs_;
};

int main(int argc, char **argv) {
  std::vector<std::string> ports = {"50051", "50052", "50053"};

  ShardedClient client(ports);

  std::cout << "---Hashing and Distributin Data ---" << std::endl;

  client.Put("apple", "red");
  client.Put("banana", "yellow");
  client.Put("grape", "purple");
  client.Put("watermelon", "green");
  client.Put("cherry", "red");

  return 0;
}