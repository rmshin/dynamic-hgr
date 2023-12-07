package(default_visibility = [
    "//visibility:public",
])

cc_binary(
    name = "run_graph_main",
    srcs = ["run_graph_main.cc"],
    data = [
        "@mediapipe//mediapipe/modules/hand_landmark:hand_landmark_full.tflite",
        "@mediapipe//mediapipe/modules/palm_detection:palm_detection_full.tflite",
        "@mediapipe//mediapipe/modules/hand_landmark:handedness.txt",
        "//models:gesture_classifier.pt",
        "//models:gestures.txt",
    ],
    deps = [
        "//src/gesture_graphs:desktop_gesture_calculators",
        "@mediapipe//mediapipe/framework:calculator_framework",
        "@mediapipe//mediapipe/framework/formats:image_frame",
        "@mediapipe//mediapipe/framework/formats:image_frame_opencv",
        "@mediapipe//mediapipe/framework/port:file_helpers",
        "@mediapipe//mediapipe/framework/port:opencv_highgui",
        "@mediapipe//mediapipe/framework/port:opencv_imgproc",
        "@mediapipe//mediapipe/framework/port:opencv_video",
        "@mediapipe//mediapipe/framework/port:parse_text_proto",
        "@mediapipe//mediapipe/framework/port:status",
        "@mediapipe//mediapipe/util:resource_util",
        "@com_google_absl//absl/flags:parse",
        "@com_google_absl//absl/log:absl_log",
    ],
)
