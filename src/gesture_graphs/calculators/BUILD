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
        "@com_google_absl//absl/log:absl_check",
        "@com_google_absl//absl/strings:str_format",
    ],
    alwayslink = 1,
)
