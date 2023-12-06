#include <string>
#include <vector>
#include <fstream>

#include "absl/log/absl_check.h"
#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/framework/port/status.h"
#include "mediapipe/framework/formats/classification.pb.h"
#include "mediapipe/util/render_data.pb.h"

constexpr char kGestureTag[] = "GESTURE";
constexpr char kHandednessTag[] = "HANDEDNESS";
constexpr char kRenderDataTag[] = "RENDER_DATA";

class GestureToRenderDataCalculator : public mediapipe::CalculatorBase
{
public:
    static absl::Status GetContract(mediapipe::CalculatorContract *cc);
    absl::Status Open(mediapipe::CalculatorContext *cc) override;
    absl::Status Process(mediapipe::CalculatorContext *cc) override;

private:
    std::vector<std::string> gesture_labels;
};
REGISTER_CALCULATOR(GestureToRenderDataCalculator);

absl::Status GestureToRenderDataCalculator::GetContract(mediapipe::CalculatorContract *cc)
{

    RET_CHECK(cc->Inputs().HasTag(kGestureTag))
        << "Must provide input stream \"GESTURE\"";
    cc->Inputs().Tag(kGestureTag).Set<int>();
    cc->Inputs().Tag(kHandednessTag).Set<std::vector<mediapipe::ClassificationList>>();
    cc->Outputs().Tag(kRenderDataTag).Set<mediapipe::RenderData>();
    return absl::OkStatus();
}

absl::Status GestureToRenderDataCalculator::Open(mediapipe::CalculatorContext *cc)
{
    cc->SetOffset(mediapipe::TimestampDiff(0));

    std::ifstream f;
    f.open("models/gestures.txt");
    ABSL_CHECK(f.is_open()) << "Missing labels file for gestures.";
    std::string line;
    while (getline(f, line))
    {
        gesture_labels.push_back(line);
    }
    f.close();

    return absl::OkStatus();
}

absl::Status GestureToRenderDataCalculator::Process(mediapipe::CalculatorContext *cc)
{
    const auto &handedness = cc->Inputs().Tag(kHandednessTag).Get<std::vector<mediapipe::ClassificationList>>();
    int gesture = cc->Inputs().Tag(kGestureTag).Get<int>();
    std::string gesture_label = gesture < gesture_labels.size() ? gesture_labels[gesture] : "Unknown";

    mediapipe::RenderData render_data;
    for (auto &hand : handedness)
    {
        size_t idx = std::distance(&handedness[0], &hand);
        auto *label_annotation = render_data.add_render_annotations();
        label_annotation->set_thickness(2.0);
        label_annotation->mutable_color()->set_r(255);
        label_annotation->mutable_color()->set_g(0);
        label_annotation->mutable_color()->set_b(0);

        auto *text = label_annotation->mutable_text();
        std::string display_text = hand.classification(0).label() + ": " + gesture_label;
        text->set_display_text(display_text);
        text->set_font_height(20);
        text->set_left(50);
        text->set_baseline(80 + 25 * idx);
    }

    // text->set_font_face(options_.font_face());
    cc->Outputs()
        .Tag(kRenderDataTag)
        .AddPacket(mediapipe::MakePacket<mediapipe::RenderData>(render_data).At(cc->InputTimestamp()));

    return absl::OkStatus();
}
