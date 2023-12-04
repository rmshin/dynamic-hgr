#include <fstream>
#include <string>
#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/framework/calculator_options.pb.h"
#include "mediapipe/framework/formats/landmark.pb.h"
#include "mediapipe/framework/formats/classification.pb.h"
#include "mediapipe/framework/port/ret_check.h"
#include "mediapipe/framework/port/status.h"
#include "capture_mode.h"
#include "absl/log/absl_check.h"
#include "absl/strings/str_format.h"

constexpr char kNormalizedLandmarksTag[] = "NORM_LANDMARKS";
constexpr char kHandednessTag[] = "HANDEDNESS";
constexpr char kCaptureModeTag[] = "CAPTURE_MODE";

class LandmarksToCsvCalculator : public mediapipe::CalculatorBase
{
public:
  static absl::Status GetContract(mediapipe::CalculatorContract *cc)
  {
    cc->Inputs().Tag(kNormalizedLandmarksTag).Set<std::vector<mediapipe::NormalizedLandmarkList>>();
    cc->Inputs().Tag(kHandednessTag).Set<std::vector<mediapipe::ClassificationList>>();
    cc->Inputs().Tag(kCaptureModeTag).Set<CaptureMode>();

    return absl::OkStatus();
  }

  absl::Status Open(mediapipe::CalculatorContext *cc) override
  {
    csvFile.open("models/landmarks.csv", std::ios::app);
    ABSL_CHECK(csvFile.is_open()) << "Error opening csv file.";
    return absl::OkStatus();
  }

  absl::Status Process(mediapipe::CalculatorContext *cc) override
  {
    if (cc->Inputs().Tag(kNormalizedLandmarksTag).IsEmpty())
    {
      return absl::OkStatus();
    }

    const auto &landmarks =
        cc->Inputs().Tag(kNormalizedLandmarksTag).Get<std::vector<mediapipe::NormalizedLandmarkList>>();
    const auto &handedness = cc->Inputs().Tag(kHandednessTag).Get<std::vector<mediapipe::ClassificationList>>();
    const auto &capture_mode = cc->Inputs().Tag(kCaptureModeTag).Get<CaptureMode>();
    WriteLandmarksToCSV(capture_mode, landmarks, handedness);

    return absl::OkStatus();
  }

  absl::Status Close(mediapipe::CalculatorContext *cc) override
  {
    csvFile.close();
    return absl::OkStatus();
  }

private:
  std::ofstream csvFile;

  void WriteLandmarksToCSV(
      const CaptureMode &capture_mode,
      const std::vector<mediapipe::NormalizedLandmarkList> &landmarks,
      const std::vector<mediapipe::ClassificationList> &handedness)
  {
    switch (capture_mode.mode)
    {
    case 0: // do nothing
      break;
    case 1: // log landmarks
      if (capture_mode.index >= 0)
      {
        // 21 landmarks * 2 hands
        size_t size = 21 * 2;
        std::vector<std::string> landmarks_arr(size);
        // default all landmarks to 0,0,0
        for (int i = 0; i < size; i++)
        {
          landmarks_arr[i] = "0.000000,0.000000,0.000000";
        }
        // add landmarks for each detected hand
        for (auto &hand : handedness)
        {
          // set array offset depending on which hand the landmarks
          // belong to - right landmarks are always placed after left
          int offset = 0;
          if (hand.classification(0).label() == "Right")
          {
            offset += 21;
          }

          // iterate over each landmark and add x, y, z coordinates to str_arr
          size_t idx = std::distance(&handedness[0], &hand);
          const auto &hand_landmarks = landmarks[idx];
          for (int j = 0; j < hand_landmarks.landmark_size(); j++)
          {
            const auto &landmark = hand_landmarks.landmark(j);
            int pos = offset + j;
            std::string landmark_str =
                absl::StrFormat("%.6f,%.6f,%.6f", landmark.x(), landmark.y(), landmark.z());
            landmarks_arr[pos++] = landmark_str;
          }
        }
        std::string landmarks_str = absl::StrJoin(landmarks_arr, ",");
        std::string line =
            std::to_string(capture_mode.index) + "," + landmarks_str + "\n";
        csvFile << line;
      }
      break;
    default:
      break;
    }
  }
};
REGISTER_CALCULATOR(LandmarksToCsvCalculator);
