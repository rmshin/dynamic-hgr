#include "torch/script.h"
#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/framework/formats/landmark.pb.h"
#include "mediapipe/framework/formats/classification.pb.h"

constexpr char kNormalizedLandmarksTag[] = "NORM_LANDMARKS";
constexpr char kHandednessTag[] = "HANDEDNESS";
constexpr char kGestureTag[] = "GESTURE";

class PtClassificationInferenceCalculator : public mediapipe::CalculatorBase
{
public:
  static absl::Status GetContract(mediapipe::CalculatorContract *cc)
  {
    cc->Inputs().Tag(kNormalizedLandmarksTag).Set<std::vector<mediapipe::NormalizedLandmarkList>>();
    cc->Inputs().Tag(kHandednessTag).Set<std::vector<mediapipe::ClassificationList>>();
    cc->Outputs().Tag(kGestureTag).Set<int>();
    return absl::OkStatus();
  }

  absl::Status Open(mediapipe::CalculatorContext *cc) override
  {
    // deserialize the ScriptModule classifier from file
    model = torch::jit::load("models/gesture_classifier.pt");
    model.eval();
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

    const std::vector<float> &landmark_floats = NormLandmarksToFloats(landmarks, handedness);
    torch::Tensor landmarks_tensor = LandmarkFloatsToTensor(landmark_floats);
    std::vector<torch::jit::IValue> inputs;
    inputs.push_back(landmarks_tensor.unsqueeze(0));

    // execute the model and turn its output into a tensor (`run_method` returns torch::jit::IValue)
    auto output = model.forward(inputs).toTensor();
    int classification = output.item<int>();
    cc->Outputs()
        .Tag(kGestureTag)
        .AddPacket(mediapipe::MakePacket<int>(classification).At(cc->InputTimestamp()));

    return absl::OkStatus();
  }

private:
  torch::jit::script::Module model;

  std::vector<float> NormLandmarksToFloats(
      const std::vector<mediapipe::NormalizedLandmarkList> &landmarks,
      const std::vector<mediapipe::ClassificationList> &handedness)
  {
    // 21 landmarks * 3 coords (x, y, z) * 2 hands
    size_t size = 21 * 3 * 2;
    std::vector<float> landmarks_arr(size, 0.0);

    // add landmarks for each detected hand
    for (auto &hand : handedness)
    {
      // set array offset depending on which hand the landmarks
      // belong to - right landmarks are always placed after left
      int offset = 0;
      if (hand.classification(0).label() == "Right")
      {
        offset += 21 * 3;
      }

      // iterate over each landmark and add x, y, z coordinates to str_arr
      size_t idx = std::distance(&handedness[0], &hand);
      const auto &hand_landmarks = landmarks[idx];
      for (int j = 0; j < hand_landmarks.landmark_size(); j++)
      {
        const auto &landmark = hand_landmarks.landmark(j);
        int pos = offset + j * 3;
        landmarks_arr[pos++] = landmark.x();
        landmarks_arr[pos++] = landmark.y();
        landmarks_arr[pos] = landmark.z();
      }
    }
    return landmarks_arr;
  }

  torch::Tensor LandmarkFloatsToTensor(const std::vector<float> &landmark_floats)
  {
    auto options = torch::TensorOptions().dtype(torch::kFloat32).requires_grad(false);
    torch::Tensor tensor = torch::tensor(landmark_floats, options);
    return tensor;
  }
};
REGISTER_CALCULATOR(PtClassificationInferenceCalculator);
