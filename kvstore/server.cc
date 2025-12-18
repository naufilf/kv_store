#include "kvstore/service_impl.h"
#include <grpcpp/grpcpp.h>
#include <iostream>
#include <memory>
#include <string>

void RunServer(int port) {
  std::string server_address = "0.0.0.0:" + std::to_string(port);
  KeyValueServiceImpl service;

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