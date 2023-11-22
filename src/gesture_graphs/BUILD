package(default_visibility = ["//visibility:public"])

exports_files(glob([
    "*.pbtxt",
]))

cc_library(
    name = "desktop_gesture_calculators",
    deps = [
        "//src/gesture_graphs/subgraphs:overlay_renderer_cpu",
        "@mediapipe//mediapipe/graphs/hand_tracking:desktop_tflite_calculators",
    ]
)