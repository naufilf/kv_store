#ifndef KVSTORE_SERVICE_IMPL_H_
#define KVSTORE_SERVICE_IMPL_H_

#include "absl/container/flat_hash_map.h"
#include "absl/synchronization/mutex.h"
#include "kvstore/protos/kv_store.grpc.pb.h"
#include "kvstore/src/storage/wal.h" // Include WAL header

// Define the class, but don't write the code for Put/Get here.
class KeyValueServiceImpl final : public kvstore::KeyValueService::Service {
public:
  // Constructor taking the WAL filename
  explicit KeyValueServiceImpl(const std::string &wal_filename);

  grpc::Status Put(grpc::ServerContext *context,
                   const kvstore::PutRequest *request,
                   kvstore::PutResponse *reply) override;

  grpc::Status Get(grpc::ServerContext *context,
                   const kvstore::GetRequest *request,
                   kvstore::GetResponse *reply) override;

private:
  absl::flat_hash_map<std::string, std::string>
      data_store_ ABSL_GUARDED_BY(map_mutex_);
  absl::Mutex map_mutex_;
  std::unique_ptr<WriteAheadLog> wal_; // Pointer to WAL instance
};

#endif // KVSTORE_SERVICE_IMPL_H_