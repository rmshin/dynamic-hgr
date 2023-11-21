cc_binary(
    name = "hand_tracking_cpu_demo",
    data = [
        "@mediapipe//mediapipe/modules/hand_landmark:hand_landmark_full.tflite",
        "@mediapipe//mediapipe/modules/palm_detection:palm_detection_full.tflite",
        "@mediapipe//mediapipe/modules/hand_landmark:handedness.txt",
    ],
    deps = [
        "@mediapipe//mediapipe/examples/desktop:demo_run_graph_main",
        "@mediapipe//mediapipe/graphs/hand_tracking:desktop_tflite_calculators",
    ],
)