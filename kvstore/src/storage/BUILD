load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "service_impl",
    srcs = ["service_impl.cc"],
    hdrs = ["service_impl.h"],
    deps = [
        ":wal",
        "//kvstore/protos:kv_store_cc_grpc",
        "@com_github_grpc_grpc//:grpc++",
        "@com_google_absl//absl/strings",
        "@com_google_absl//absl/container:flat_hash_map",
        "@com_google_absl//absl/synchronization",
    ],
)

cc_library(
    name = "wal",
    srcs = ["wal.cc"],
    hdrs = ["wal.h"],
    deps = ["//kvstore/protos:wal_cc_proto"],
)














