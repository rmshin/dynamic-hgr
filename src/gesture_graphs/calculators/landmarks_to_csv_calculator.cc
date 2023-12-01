#include <fstream>
#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/framework/calculator_options.pb.h"
#include "mediapipe/framework/formats/landmark.pb.h"
#include "mediapipe/framework/formats/classification.pb.h"
#include "mediapipe/framework/port/ret_check.h"
#include "mediapipe/framework/port/status.h"
#include "capture_mode.h"

constexpr char kNormalizedLandmarksTag[] = "NORM_LANDMARKS";
constexpr char kCaptureModeTag[] = "CAPTURE_MODE";

class LandmarksToCsvCalculator : public mediapipe::CalculatorBase
{
public:
  static absl::Status GetContract(mediapipe::CalculatorContract *cc)
  {
    cc->Inputs().Tag(kNormalizedLandmarksTag).Set<std::vector<mediapipe::NormalizedLandmarkList>>();
    cc->Inputs().Tag("HANDEDNESS").Set<std::vector<mediapipe::ClassificationList>>();
    cc->Inputs().Tag(kCaptureModeTag).Set<CaptureMode>();

    return absl::OkStatus();
  }

  absl::Status Open(mediapipe::CalculatorContext *cc) override
  {
    std::ofstream csvFile;
    return absl::OkStatus();
  }

  absl::Status Process(mediapipe::CalculatorContext *cc) override
  {
    if (cc->Inputs().Tag(kNormalizedLandmarksTag).IsEmpty())
    {
      return absl::OkStatus();
    }

    const auto &landmarks =
        cc->Inputs().Tag(kNormalizedLandmarksTag).Get<std::vector<mediapipe::NormalizedLandmarkList>>().front();

    CaptureMode capture_mode = cc->Inputs().Tag(kCaptureModeTag).Get<CaptureMode>();
    printf("record mode: %d, record index: %d\n", capture_mode.mode, capture_mode.index);
    WriteLandmarksToCSV(&capture_mode);

    // printf("---------START LANDMARKS---------\n");
    // for (int i = 0; i < landmarks.landmark_size(); i++)
    // {
    //   printf("x: %f, y: %f, z: %f\n", landmarks.landmark(i).x(), landmarks.landmark(i).y(), landmarks.landmark(i).z());
    // }
    // printf("----------END LANDMARKS----------\n");
    return absl::OkStatus();
  }

private:
  void WriteLandmarksToCSV(CaptureMode *capture_mode) {}
};
REGISTER_CALCULATOR(LandmarksToCsvCalculator);
