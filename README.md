### Running hand tracking demo

Build command:

```
bazel build -c opt --verbose_failures --experimental_repo_remote_exec --define MEDIAPIPE_DISABLE_GPU=1 \
    src:run_graph_main
```

Notes:

- `MEDIAPIPE_DISABLE_GPU=1` must be set during build as MediaPipe only supports GPU inference for Linux at the moment.
- The `--experimental_repo_remote_exec` flag needs to be passed to properly build the project based on external dependencies such as MediaPipe & Tensorflow without relying on local copies/installations to be in place.

Run command:

```
GLOG_logtostderr=1 bazel-bin/src/run_graph_main --calculator_graph_config_file=src/gesture_graphs/gesture_recognition_cpu.pbtxt --resource_root_dir=bazel-bin/src/run_graph_main.runfiles/mediapipe
```

Notes:

- The demo binary expects a `--calculator_graph_config_file` flag that passes the relevant `.pbtxt` graph config file path to the graph runner.
- By default, the `PalmDetectionModelLoader` calculator that gets called by the parent `HandLandmarkTrackingCpu` graph will attempt to load the model at `mediapipe/modules/palm_detection/palm_detection_full.tflite`.
  - This will fail in cases where mediapipe is not directly installed within the project directory, and so the `--resource_root_dir` flag must be used to prepend a root directory prefix to the default model path.
  - In this case, the models are made available within the `bazel-bin/path/to/bin.runfiles` via the `data` dependencies for `run_graph_main.cc` defined in `src/BUILD`.
