package(default_visibility = ["//visibility:public"])

cc_library(
    name = "landmarks_to_csv_calculator",
    srcs = ["landmarks_to_csv_calculator.cc", "capture_mode.h"],
    visibility = ["//visibility:public"],
    deps = [
        "@mediapipe//mediapipe/framework:calculator_framework",
        "@mediapipe//mediapipe/framework:calculator_options_cc_proto",
        "@mediapipe//mediapipe/framework/formats:classification_cc_proto",
        "@mediapipe//mediapipe/framework/formats:landmark_cc_proto",
        "@mediapipe//mediapipe/framework/port:ret_check",
        "@mediapipe//mediapipe/framework/port:status",
    ],
    alwayslink = 1,
)
