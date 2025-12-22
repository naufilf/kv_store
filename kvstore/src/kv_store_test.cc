#include "kvstore/src/storage/service_impl.h"
#include "gtest/gtest.h"

TEST(KeyValueServiceTest, PutStoresValue) {
  KeyValueServiceImpl service("test_wal.log");
  grpc::ServerContext context;

  kvstore::PutRequest request;
  request.set_key("test_key");
  request.set_value("test_value");

  kvstore::PutResponse response;

  grpc::Status status = service.Put(&context, &request, &response);

  EXPECT_TRUE(status.ok());
  EXPECT_TRUE(response.success());
}

TEST(KeyValueServiceTest, GetRetrievesValue) {
  KeyValueServiceImpl service("test_wal.log");
  grpc::ServerContext context;

  kvstore::PutRequest put_req;
  put_req.set_key("my_key");
  put_req.set_value("my_value");
  kvstore::PutResponse put_res;
  service.Put(&context, &put_req, &put_res);

  // Now try to get it back
  kvstore::GetRequest get_req;
  get_req.set_key("my_key");
  kvstore::GetResponse get_res;

  grpc::Status status = service.Get(&context, &get_req, &get_res);

  EXPECT_TRUE(status.ok());
  EXPECT_TRUE(get_res.found());
  EXPECT_EQ(get_res.value(), "my_value");
}

TEST(KeyValueServiceTest, PersistsAndRecovers) {
  std::string wal_file = "recovery_test.wal";
  // Clean up previous run
  remove(wal_file.c_str());

  {
    KeyValueServiceImpl service(wal_file);
    grpc::ServerContext context;
    kvstore::PutRequest req;
    req.set_key("persist_key");
    req.set_value("persist_value");
    kvstore::PutResponse res;
    service.Put(&context, &req, &res);
  } // Service destroyed here

  {
    // New service instance, same file
    KeyValueServiceImpl service(wal_file);
    grpc::ServerContext context;
    kvstore::GetRequest req;
    req.set_key("persist_key");
    kvstore::GetResponse res;
    service.Get(&context, &req, &res);

    EXPECT_TRUE(res.found());
    EXPECT_EQ(res.value(), "persist_value");
  }
  // Clean up
  remove(wal_file.c_str());
}