load("@rules_proto//proto:defs.bzl", "proto_library")
load("@com_github_grpc_grpc//bazel:cc_grpc_library.bzl", "cc_grpc_library")

proto_library(
    name = "kv_store_proto",
    srcs = ["kv_store.proto"],
)

cc_proto_library(
    name = "kv_store_cc_proto",
    deps = [":kv_store_proto"],
)

cc_grpc_library(
    name = "kv_store_cc_grpc",
    srcs = [":kv_store_proto"],
    grpc_only = True,
    deps = [":kv_store_cc_proto"]
)

cc_library(
    name = "service_impl",
    srcs = ["service_impl.cc"],
    hdrs = ["service_impl.h"],
    deps = [
        ":kv_store_cc_grpc",
        "@com_github_grpc_grpc//:grpc++",
        "@com_google_absl//absl/strings",
        "@com_google_absl//absl/container:flat_hash_map",
        "@com_google_absl//absl/synchronization",
    ],
)

cc_binary(
    name = "server",
    srcs = ["server.cc"],
    deps = [
        ":service_impl", # Uses the library above
        "@com_github_grpc_grpc//:grpc++",
    ],
)

cc_binary(
    name = "client",
    srcs = ["client.cc"],
    deps = [
        ":kv_store_cc_grpc",              # The auto-generated gRPC stub
        "@com_github_grpc_grpc//:grpc++", # The gRPC C++ library
    ],
)

cc_test(
    name = "kv_store_test",
    srcs = ["kv_store_test.cc"],
    deps = [
        ":service_impl",  # <--- This dependency allows the test to "see" the header
        "@com_google_googletest//:gtest_main",
        "@com_github_grpc_grpc//:grpc++",
    ],
)