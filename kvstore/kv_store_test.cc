#include "kvstore/service_impl.h"
#include "gtest/gtest.h"

TEST(KeyValueServiceTest, PutStoresValue) {
  KeyValueServiceImpl service;
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
  KeyValueServiceImpl service;
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