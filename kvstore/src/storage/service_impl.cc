#include "kvstore/src/storage/service_impl.h"
#include "absl/strings/str_cat.h"
#include <iostream>

using grpc::ServerContext;
using grpc::Status;

// Constructor: Initialize WAL and recover state
KeyValueServiceImpl::KeyValueServiceImpl(const std::string &wal_filename) {
  wal_ = std::make_unique<WriteAheadLog>(wal_filename);
  
  std::cout << "Recovering from WAL..." << std::endl;
  // Replay the log to restore the map
  wal_->ReadAll([this](const kvstore::LogEntry &entry) {
    // We don't need to lock here because this runs in the constructor 
    // before the server starts listening (single-threaded context).
    if (entry.op() == kvstore::LogEntry::PUT) {
        data_store_[entry.key()] = entry.value();
    }
    // Handle DELETE op if we implement it later
  });
  std::cout << "Recovery complete. Loaded " << data_store_.size() << " keys." << std::endl;
}

Status KeyValueServiceImpl::Put(ServerContext *context,
                                const kvstore::PutRequest *request,
                                kvstore::PutResponse *reply) {
  absl::MutexLock lock(&map_mutex_);

  // 1. Write to WAL first (Durability)
  // If this fails (e.g., disk full), we should fail the request.
  if (!wal_->Append(request->key(), request->value())) {
      return Status(grpc::StatusCode::INTERNAL, "Failed to write to WAL");
  }

  // 2. Update in-memory Store
  data_store_[request->key()] = request->value();

  reply->set_success(true);
  reply->set_message(absl::StrCat("Stored key: ", request->key()));
  return Status::OK;
}

Status KeyValueServiceImpl::Get(ServerContext *context,
                                const kvstore::GetRequest *request,
                                kvstore::GetResponse *reply) {
  absl::MutexLock lock(&map_mutex_);
  auto it = data_store_.find(request->key());

  if (it != data_store_.end()) {
    reply->set_value(it->second);
    reply->set_found(true);
  } else {
    reply->set_found(false);
  }
  return Status::OK;
}