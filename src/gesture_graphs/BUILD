package(default_visibility = ["//visibility:public"])

exports_files(glob([
    "*.pbtxt",
]))

cc_library(
    name = "desktop_gesture_calculators",
    deps = [
        "//src/gesture_graphs/calculators:landmarks_to_csv_calculator",
        "//src/gesture_graphs/calculators:pt_classification_inference_calculator",
        "//src/gesture_graphs/calculators:gesture_to_render_data_calculator",
        "//src/gesture_graphs/subgraphs:overlay_renderer_cpu",
        "@mediapipe//mediapipe/graphs/hand_tracking:desktop_tflite_calculators",
    ]
)