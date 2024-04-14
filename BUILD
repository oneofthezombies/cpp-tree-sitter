cc_library(
    name = "tree_sitter",
    srcs = [
        "third_party/tree-sitter-0.22.2/lib/src/lib.c",
    ],
    hdrs = [
        "third_party/tree-sitter-0.22.2/lib/include/tree_sitter/api.h",
    ] + glob(
        [
            "third_party/tree-sitter-0.22.2/lib/src/**/*.c",
            "third_party/tree-sitter-0.22.2/lib/src/**/*.h",
        ],
        ["third_party/tree-sitter-0.22.2/lib/src/lib.c"],
    ),
    copts = [
        "-std=c17",
    ],
    includes = [
        "third_party/tree-sitter-0.22.2/lib/include",
        "third_party/tree-sitter-0.22.2/lib/src",
    ],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "cpp_tree_sitter",
    srcs = ["src/cpp_tree_sitter/api.cc"],
    hdrs = [
        "src/cpp_tree_sitter/api.h",
    ],
    copts = [
        "-std=c++20",
        "-fno-rtti",
    ],
    includes = [
        "src",
    ],
    visibility = ["//visibility:public"],
    deps = [
        ":tree_sitter",
    ],
)
