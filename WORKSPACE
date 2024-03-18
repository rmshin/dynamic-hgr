workspace(name = "dynamic_hgr")

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

# http_archive(
#     name = "macos_opencv",
#     strip_prefix = "opencv-3.4.10",
#     urls = ["https://github.com/opencv/opencv/archive/3.4.10.tar.gz"],
#     build_file = "@//third_party/opencv:BUILD",
# )

new_local_repository(
    name = "macos_opencv",
    build_file = "@//third_party/opencv:BUILD",
    # path = "/usr/local", # intel macs
    path = "/opt/homebrew", # MX macs
)

http_archive(
    name = "libtorch",
    build_file = "@//third_party/libtorch:BUILD",
    strip_prefix = "libtorch",
    # urls = ["https://download.pytorch.org/libtorch/cpu/libtorch-macos-2.1.1.zip"], # intel macs
    urls = ["https://github.com/mlverse/libtorch-mac-m1/releases/download/LibTorchOpenMP/libtorch-v2.1.0.zip"], # MX macs
)

http_archive(
    name = "mediapipe",
    urls = ["https://github.com/google/mediapipe/archive/refs/tags/v0.10.8.tar.gz"],
    sha256 = "c4554f9f41192202263c19cc0f6e330b42f003bbfc5ce16ba727ab4c71ad6e5d",
    strip_prefix = "mediapipe-0.10.8",
)

# ABSL cpp library lts_2023_08_02.
http_archive(
    name = "com_google_absl",
    urls = [
        "https://github.com/abseil/abseil-cpp/archive/refs/tags/20230802.1.tar.gz",
    ],
    strip_prefix = "abseil-cpp-20230802.1",
    sha256 = "987ce98f02eefbaf930d6e38ab16aa05737234d7afbab2d5c4ea7adbe50c28ed"
)

http_archive(
    name = "bazel_skylib",
    sha256 = "74d544d96f4a5bb630d465ca8bbcfe231e3594e5aae57e1edbf17a6eb3ca2506",
    urls = [
        "https://storage.googleapis.com/mirror.tensorflow.org/github.com/bazelbuild/bazel-skylib/releases/download/1.3.0/bazel-skylib-1.3.0.tar.gz",
        "https://github.com/bazelbuild/bazel-skylib/releases/download/1.3.0/bazel-skylib-1.3.0.tar.gz",
    ],
)
load("@bazel_skylib//:workspace.bzl", "bazel_skylib_workspace")
bazel_skylib_workspace()
load("@bazel_skylib//lib:versions.bzl", "versions")
versions.check(minimum_bazel_version = "3.7.2")

http_archive(
    name = "build_bazel_apple_support",
    sha256 = "a8ba6fd09d0ffeba9b5f398f3a46262470fe0addddb4ef5afa7eab18d001a7b0",
    url = "https://github.com/bazelbuild/apple_support/releases/download/1.14.0/apple_support.1.14.0.tar.gz",
)

load(
    "@build_bazel_apple_support//lib:repositories.bzl",
    "apple_support_dependencies",
)

apple_support_dependencies()

http_archive(
    name = "rules_cc",
    strip_prefix = "rules_cc-0.0.9",
    sha256 = "2037875b9a4456dce4a79d112a8ae885bbc4aad968e6587dca6e64f3a0900cdf",
    urls = ["https://github.com/bazelbuild/rules_cc/releases/download/0.0.9/rules_cc-0.0.9.tar.gz"],
)

http_archive(
    name = "rules_foreign_cc",
    sha256 = "2a4d07cd64b0719b39a7c12218a3e507672b82a97b98c6a89d38565894cf7c51",
    strip_prefix = "rules_foreign_cc-0.9.0",
    url = "https://github.com/bazelbuild/rules_foreign_cc/archive/refs/tags/0.9.0.tar.gz",
)

load("@rules_foreign_cc//foreign_cc:repositories.bzl", "rules_foreign_cc_dependencies")

rules_foreign_cc_dependencies()

# Node dependencies
http_archive(
    name = "build_bazel_rules_nodejs",
    sha256 = "94070eff79305be05b7699207fbac5d2608054dd53e6109f7d00d923919ff45a",
    urls = ["https://github.com/bazelbuild/rules_nodejs/releases/download/5.8.2/rules_nodejs-5.8.2.tar.gz"],
)

load("@build_bazel_rules_nodejs//:repositories.bzl", "build_bazel_rules_nodejs_dependencies")
build_bazel_rules_nodejs_dependencies()

# fetches nodejs, npm, and yarn
load("@build_bazel_rules_nodejs//:index.bzl", "node_repositories", "yarn_install")
node_repositories()
yarn_install(
    name = "npm",
    package_json = "@mediapipe//:package.json",
    yarn_lock = "@mediapipe//:yarn.lock",
)


# Protobuf for Node dependencies
http_archive(
    name = "rules_proto_grpc",
    sha256 = "bbe4db93499f5c9414926e46f9e35016999a4e9f6e3522482d3760dc61011070",
    strip_prefix = "rules_proto_grpc-4.2.0",
    urls = ["https://github.com/rules-proto-grpc/rules_proto_grpc/archive/4.2.0.tar.gz"],
)

http_archive(
    name = "com_google_protobuf_javascript",
    sha256 = "35bca1729532b0a77280bf28ab5937438e3dcccd6b31a282d9ae84c896b6f6e3",
    strip_prefix = "protobuf-javascript-3.21.2",
    urls = ["https://github.com/protocolbuffers/protobuf-javascript/archive/refs/tags/v3.21.2.tar.gz"],
)

load("@rules_proto_grpc//:repositories.bzl", "rules_proto_grpc_toolchains", "rules_proto_grpc_repos")
rules_proto_grpc_toolchains()
rules_proto_grpc_repos()

load("@rules_proto//proto:repositories.bzl", "rules_proto_dependencies", "rules_proto_toolchains")
rules_proto_dependencies()
rules_proto_toolchains()

load("@mediapipe//third_party:external_files.bzl", "external_files")
external_files()

load("@mediapipe//third_party:wasm_files.bzl", "wasm_files")
wasm_files()


# Needed by TensorFlow
http_archive(
    name = "io_bazel_rules_closure",
    sha256 = "e0a111000aeed2051f29fcc7a3f83be3ad8c6c93c186e64beb1ad313f0c7f9f9",
    strip_prefix = "rules_closure-cf1e44edb908e9616030cc83d085989b8e6cd6df",
    urls = [
        "http://mirror.tensorflow.org/github.com/bazelbuild/rules_closure/archive/cf1e44edb908e9616030cc83d085989b8e6cd6df.tar.gz",
        "https://github.com/bazelbuild/rules_closure/archive/cf1e44edb908e9616030cc83d085989b8e6cd6df.tar.gz",  # 2019-04-04
    ],
)

# TensorFlow repo should always go after the other external dependencies.
# TF on 2023-07-26.
_TENSORFLOW_GIT_COMMIT = "e92261fd4cec0b726692081c4d2966b75abf31dd"
# curl -L https://github.com/tensorflow/tensorflow/archive/<TENSORFLOW_GIT_COMMIT>.tar.gz | shasum -a 256
_TENSORFLOW_SHA256 = "478a229bd4ec70a5b568ac23b5ea013d9fca46a47d6c43e30365a0412b9febf4"
http_archive(
    name = "org_tensorflow",
    urls = [
      "https://github.com/tensorflow/tensorflow/archive/%s.tar.gz" % _TENSORFLOW_GIT_COMMIT,
    ],
    patches = [
        "@mediapipe//third_party:org_tensorflow_compatibility_fixes.diff",
        "@mediapipe//third_party:org_tensorflow_system_python.diff",
        # Diff is generated with a script, don't update it manually.
        "@mediapipe//third_party:org_tensorflow_custom_ops.diff",
        # Works around Bazel issue with objc_library.
        # See https://github.com/bazelbuild/bazel/issues/19912
        "@mediapipe//third_party:org_tensorflow_objc_build_fixes.diff",
    ],
    patch_args = [
        "-p1",
    ],
    strip_prefix = "tensorflow-%s" % _TENSORFLOW_GIT_COMMIT,
    sha256 = _TENSORFLOW_SHA256,
)

load("@org_tensorflow//tensorflow:workspace3.bzl", "tf_workspace3")
tf_workspace3()
load("@org_tensorflow//tensorflow:workspace2.bzl", "tf_workspace2")
tf_workspace2()