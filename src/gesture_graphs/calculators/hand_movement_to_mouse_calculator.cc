#include <tuple>
#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/framework/formats/landmark.pb.h"
#include "mediapipe/framework/formats/classification.pb.h"
#include "src/mouse/mouse_controller.h"
#include "landmark_xy.h"

constexpr char kNormalizedLandmarksTag[] = "NORM_LANDMARKS";
constexpr char kHandednessTag[] = "HANDEDNESS";
constexpr char kGestureTag[] = "GESTURE";
constexpr char kPrevLandmarkXyTag[] = "PREV_LANDMARK_XY";
constexpr char kLandmarkXyTag[] = "LANDMARK_XY";

class HandMovementToMouseCalculator : public mediapipe::CalculatorBase
{
public:
    static absl::Status GetContract(mediapipe::CalculatorContract *cc)
    {
        cc->Inputs().Tag(kNormalizedLandmarksTag).Set<std::vector<mediapipe::NormalizedLandmarkList>>();
        cc->Inputs().Tag(kHandednessTag).Set<std::vector<mediapipe::ClassificationList>>();
        cc->Inputs().Tag(kGestureTag).Set<int>();
        cc->Inputs().Tag(kPrevLandmarkXyTag).Set<LandmarkXY>();
        cc->Outputs().Tag(kLandmarkXyTag).Set<LandmarkXY>();
        return absl::OkStatus();
    }

    absl::Status Open(mediapipe::CalculatorContext *cc) override
    {
        mouseController = [[MouseController alloc] init];
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
        auto [landmark_x, landmark_y] = getXyFromLandmarks(landmarks, handedness);
        if (!cc->Inputs().Tag(kPrevLandmarkXyTag).IsEmpty())
        {
            const auto gesture = cc->Inputs().Tag(kGestureTag).Get<int>();
            const auto &prev_landmark = cc->Inputs().Tag(kPrevLandmarkXyTag).Get<LandmarkXY>();

            // mappings in models/gesture.txt
            if (gesture == 1) // Cursor
            {
                NSArray *coords = [mouseController getCurrentMouseXY];
                if ([coords count] == 2)
                {
                    NSNumber *mouse_x = coords[0];
                    NSNumber *mouse_y = coords[1];

                    float prev_x = [mouse_x floatValue];
                    float prev_y = [mouse_y floatValue];

                    float delta_x = (landmark_x - prev_landmark.x) * [mouseController width] * 2;
                    float delta_y = (landmark_y - prev_landmark.y) * [mouseController height] * 2;
                    [mouseController moveMouseToX:(prev_x + delta_x) Y:(prev_y + delta_y)];
                }
            }
            else if (gesture == 0 || gesture == 2) // None || Stop
            {
            }
            else if (gesture == 3) // Navigate
            {
            }
        }

        cc->Outputs()
            .Tag(kLandmarkXyTag)
            .AddPacket(mediapipe::MakePacket<LandmarkXY>(landmark_x, landmark_y).At(cc->InputTimestamp()));

        return absl::OkStatus();
    }

private:
    MouseController *mouseController;

    std::tuple<float, float> getXyFromLandmarks(
        const std::vector<mediapipe::NormalizedLandmarkList> &landmarks,
        const std::vector<mediapipe::ClassificationList> &handedness)
    {
        // FIXME: hack that returns xy coords of highest index finger landmark
        // https://developers.google.com/mediapipe/solutions/vision/hand_landmarker#models)
        float landmark_x = landmarks[0].landmark(8).x(),
              landmark_y = landmarks[0].landmark(8).y();
        if (landmarks.size() > 1 && landmarks[1].landmark(8).y() > landmark_y)
        {
            landmark_x = landmarks[1].landmark(8).x();
            landmark_y = landmarks[1].landmark(8).y();
        }

        return {landmark_x, landmark_y};
    }
};
REGISTER_CALCULATOR(HandMovementToMouseCalculator);
