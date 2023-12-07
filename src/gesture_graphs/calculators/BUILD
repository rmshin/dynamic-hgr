package(default_visibility = ["//visibility:public"])

cc_library(
    name = "landmarks_to_csv_calculator",
    srcs = ["landmarks_to_csv_calculator.cc", "capture_mode.h"],
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

cc_library(
    name = "pt_classification_inference_calculator",
    srcs = ["pt_classification_inference_calculator.cc"],
    deps = [
        "@mediapipe//mediapipe/framework:calculator_framework",
        "@mediapipe//mediapipe/framework/formats:classification_cc_proto",
        "@mediapipe//mediapipe/framework/formats:landmark_cc_proto",
        "@libtorch//:libtorch",
    ],
    alwayslink = 1,
)

cc_library(
    name = "gesture_to_render_data_calculator",
    srcs = ["gesture_to_render_data_calculator.cc"],
    deps = [
        "@mediapipe//mediapipe/framework:calculator_framework",
        "@mediapipe//mediapipe/framework/formats:classification_cc_proto",
        "@mediapipe//mediapipe/framework/port:status",
        "@mediapipe//mediapipe/util:render_data_cc_proto",
        "@com_google_absl//absl/log:absl_check",
    ],
    alwayslink = 1,
)

cc_library(
    name = "hand_movement_to_mouse_calculator",
    srcs = ["hand_movement_to_mouse_calculator.cc", "landmark_xy.h"],
    deps = [
        "//src/mouse:mouse_controller",
        "@mediapipe//mediapipe/framework:calculator_framework",
        "@mediapipe//mediapipe/framework/formats:classification_cc_proto",
        "@mediapipe//mediapipe/framework/formats:landmark_cc_proto",
    ],
    copts = ["-ObjC++"],
    linkopts = ["-framework CoreGraphics"],
    alwayslink = 1,
)
