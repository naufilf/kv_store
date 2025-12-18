#include "kvstore/service_impl.h"
#include "absl/strings/str_cat.h"

using grpc::ServerContext;
using grpc::Status;

Status KeyValueServiceImpl::Put(ServerContext *context,
                                const kvstore::PutRequest *request,
                                kvstore::PutResponse *reply) {
  absl::MutexLock lock(&map_mutex_);
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