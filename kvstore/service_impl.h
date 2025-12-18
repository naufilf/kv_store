#ifndef KVSTORE_SERVICE_IMPL_H_
#define KVSTORE_SERVICE_IMPL_H_

#include "kvstore/kv_store.grpc.pb.h"
#include "absl/container/flat_hash_map.h"
#include "absl/synchronization/mutex.h"

// Define the class, but don't write the code for Put/Get here.
class KeyValueServiceImpl final : public kvstore::KeyValueService::Service {
 public:
  grpc::Status Put(grpc::ServerContext* context, const kvstore::PutRequest* request,
                   kvstore::PutResponse* reply) override;

  grpc::Status Get(grpc::ServerContext* context, const kvstore::GetRequest* request,
                   kvstore::GetResponse* reply) override;

 private:
  absl::flat_hash_map<std::string, std::string> data_store_ ABSL_GUARDED_BY(map_mutex_);
  absl::Mutex map_mutex_;
};

#endif  // KVSTORE_SERVICE_IMPL_H_