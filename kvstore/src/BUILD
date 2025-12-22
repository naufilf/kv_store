load("@rules_proto//proto:defs.bzl", "proto_library")
load("@com_github_grpc_grpc//bazel:cc_grpc_library.bzl", "cc_grpc_library")

cc_binary(
    name = "server",
    srcs = ["server.cc"],
    deps = [
        "//kvstore/src/storage:service_impl",
        "@com_github_grpc_grpc//:grpc++",
        "//kvstore/src/storage:wal",
    ],
)

cc_binary(
    name = "client",
    srcs = ["client.cc"],
    deps = [
        "//kvstore/protos:kv_store_cc_grpc",              # The auto-generated gRPC stub
        "@com_github_grpc_grpc//:grpc++", # The gRPC C++ library
    ],
)

cc_test(
    name = "kv_store_test",
    srcs = ["kv_store_test.cc"],
    deps = [
        "//kvstore/src/storage:service_impl",  
        "@com_google_googletest//:gtest_main",
        "@com_github_grpc_grpc//:grpc++",
    ],
)