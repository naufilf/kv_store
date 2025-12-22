#include "kvstore/src/storage/service_impl.h"
#include <grpcpp/grpcpp.h>
#include <iostream>
#include <memory>
#include <string>

void RunServer(int port) {
  std::string server_address = "0.0.0.0:" + std::to_string(port);
  // Create service with a unique Write-Ahead Log file per port
  std::string wal_file = "kvstore_" + std::to_string(port) + ".wal";
  KeyValueServiceImpl service(wal_file);

  grpc::ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);

  std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;
  server->Wait();
}

int main(int argc, char **argv) {
  int port = 50051; // Default port

  if (argc > 1) {
    port = std::stoi(argv[1]);
  }

  RunServer(port);
  return 0;
}