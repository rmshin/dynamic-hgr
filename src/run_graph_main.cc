// Copyright 2019 The MediaPipe Authors.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// An example of sending OpenCV webcam frames into a MediaPipe graph.
#include <cstdlib>
#include <tuple>

#include "gesture_graphs/calculators/capture_mode.h"
#include "absl/flags/parse.h"
#include "absl/log/absl_log.h"
#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/framework/formats/image_frame.h"
#include "mediapipe/framework/formats/image_frame_opencv.h"
#include "mediapipe/framework/port/file_helpers.h"
#include "mediapipe/framework/port/opencv_highgui_inc.h"
#include "mediapipe/framework/port/opencv_imgproc_inc.h"
#include "mediapipe/framework/port/opencv_video_inc.h"
#include "mediapipe/framework/port/parse_text_proto.h"
#include "mediapipe/framework/port/status.h"
#include "mediapipe/util/resource_util.h"

constexpr char kInputStream[] = "input_video";
constexpr char kRecordModeInputStream[] = "capture_mode";
constexpr char kOutputStream[] = "output_video";
constexpr char kWindowName[] = "MediaPipe";

void set_capture_mode(int key, int *curr_mode, int *curr_num)
{
  if ('0' <= key && key <= '9') // 0 - 9
  {
    *curr_num = key - '0';
  }
  else
  {
    *curr_num = -1;
    if (key == 'n') // no capture
    {
      *curr_mode = 0;
    }
    else if (key == 'k') // landmarks
    {
      *curr_mode = 1;
    }
  }
}

absl::Status RunMPPGraph()
{
  std::string calculator_graph_config_contents;
  MP_RETURN_IF_ERROR(mediapipe::file::GetContents(
      "src/gesture_graphs/gesture_recognition_cpu.pbtxt",
      &calculator_graph_config_contents));
  ABSL_LOG(INFO) << "Get calculator graph config contents: "
                 << calculator_graph_config_contents;
  mediapipe::CalculatorGraphConfig config =
      mediapipe::ParseTextProtoOrDie<mediapipe::CalculatorGraphConfig>(
          calculator_graph_config_contents);

  ABSL_LOG(INFO) << "Initialize the calculator graph.";
  mediapipe::CalculatorGraph graph;
  MP_RETURN_IF_ERROR(graph.Initialize(config));

  ABSL_LOG(INFO) << "Initialize the camera or load the video.";
  cv::VideoCapture capture;
  capture.open(0);
  RET_CHECK(capture.isOpened());

  cv::namedWindow(kWindowName, /*flags=WINDOW_AUTOSIZE*/ 1);
#if (CV_MAJOR_VERSION >= 3) && (CV_MINOR_VERSION >= 2)
  capture.set(cv::CAP_PROP_FRAME_WIDTH, 640);
  capture.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
  capture.set(cv::CAP_PROP_FPS, 30);
  ABSL_LOG(INFO) << "Set camera properties.";
#endif

  ABSL_LOG(INFO) << "Start running the calculator graph.";
  MP_ASSIGN_OR_RETURN(mediapipe::OutputStreamPoller poller,
                      graph.AddOutputStreamPoller(kOutputStream));
  MP_RETURN_IF_ERROR(graph.StartRun({}));

  ABSL_LOG(INFO) << "Start grabbing and processing frames.";
  bool grab_frames = true;
  // for logging landmarks
  int capture_mode = 0, capture_idx = -1;
  while (grab_frames)
  {
    // Capture opencv camera or video frame.
    cv::Mat camera_frame_raw;
    capture >> camera_frame_raw;
    if (camera_frame_raw.empty())
    {
      ABSL_LOG(INFO) << "Ignore empty frames from camera.";
      continue;
    }
    cv::Mat camera_frame;
    cv::cvtColor(camera_frame_raw, camera_frame, cv::COLOR_BGR2RGB);
    cv::flip(camera_frame, camera_frame, /*flipcode=HORIZONTAL*/ 1);

    // Wrap Mat into an ImageFrame.
    auto input_frame = absl::make_unique<mediapipe::ImageFrame>(
        mediapipe::ImageFormat::SRGB, camera_frame.cols, camera_frame.rows,
        mediapipe::ImageFrame::kDefaultAlignmentBoundary);
    cv::Mat input_frame_mat = mediapipe::formats::MatView(input_frame.get());
    camera_frame.copyTo(input_frame_mat);

    // Send image packet into the graph.
    size_t frame_timestamp_us =
        (double)cv::getTickCount() / (double)cv::getTickFrequency() * 1e6;
    MP_RETURN_IF_ERROR(graph.AddPacketToInputStream(
        kInputStream, mediapipe::Adopt(input_frame.release())
                          .At(mediapipe::Timestamp(frame_timestamp_us))));
    // Send record mode & idx to graph
    MP_RETURN_IF_ERROR(graph.AddPacketToInputStream(
        kRecordModeInputStream, mediapipe::MakePacket<CaptureMode>(capture_mode, capture_idx).At(mediapipe::Timestamp(frame_timestamp_us))));
    // Get the graph result packet, or stop if that fails.
    mediapipe::Packet packet;
    if (!poller.Next(&packet))
      break;
    auto &output_frame = packet.Get<mediapipe::ImageFrame>();

    // Convert back to opencv for display or saving.
    cv::Mat output_frame_mat = mediapipe::formats::MatView(&output_frame);
    cv::cvtColor(output_frame_mat, output_frame_mat, cv::COLOR_RGB2BGR);

    cv::imshow(kWindowName, output_frame_mat);
    // Press ESC key to exit.
    const int pressed_key = cv::waitKey(5);
    if (pressed_key == 27)
      grab_frames = false;
    // Assign key presses for capturing gesture training data
    set_capture_mode(pressed_key, &capture_mode, &capture_idx);
  }

  ABSL_LOG(INFO) << "Shutting down.";
  MP_RETURN_IF_ERROR(graph.CloseInputStream(kInputStream));
  MP_RETURN_IF_ERROR(graph.CloseInputStream(kRecordModeInputStream));
  return graph.WaitUntilDone();
}

int main(int argc, char **argv)
{
  google::InitGoogleLogging(argv[0]);
  absl::ParseCommandLine(argc, argv);
  absl::Status run_status = RunMPPGraph();
  if (!run_status.ok())
  {
    ABSL_LOG(ERROR) << "Failed to run the graph: " << run_status.message();
    return EXIT_FAILURE;
  }
  else
  {
    ABSL_LOG(INFO) << "Success!";
  }
  return EXIT_SUCCESS;
}
