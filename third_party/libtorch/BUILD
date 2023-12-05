package(default_visibility = ["//visibility:public"])

cc_library(
    name = "libtorch",
    srcs = select({
        "@bazel_tools//src/conditions:darwin": glob([
          "lib/libcaffe2.dylib", 
          "lib/libc10.dylib",
          "lib/libiomp5.dylib",
          "lib/libtorch.dylib", 
          "lib/libtorch_cpu.dylib", 
          "lib/libtorch.1.dylib", 
          "lib/libtensorpipe.dylib",
        ]),
        "//conditions:default": glob(["lib/lib*.so*"], exclude=["lib/libtorch_python.so", "lib/libnnapi_backend.so"]),
    }),
    hdrs = glob([
        "include/ATen/**",
        "include/c10/**",
        "include/caffe2/**",
        "include/torch/**",
        "include/torch/csrc/**",
        "include/torch/csrc/jit/**",
        "include/torch/csrc/api/include/**",
    ]),
    includes = [
        "include",
        "include/torch/csrc/api/include",
    ],
)
