// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Track_Element_Extended.hpp"
#include <VehicleArena/Math/Transformation/Tait_Bryan_Angles.hpp>

using namespace VA;

TrackElementExtended TrackElementExtended::create(
    const std::optional<TrackElementExtended>& predecessor,
    const TrackElement& track_element)
{
    TrackElementExtended result;
    result.element = track_element;
    if (!predecessor.has_value()) {
        result.meters_to_start = 0.;
    } else {
        auto ds = std::sqrt(sum(squared(
            predecessor->transformation().position -
            result.transformation().position)));
        result.meters_to_start = predecessor->meters_to_start + ds;
    }
    return result;
}

const OffsetAndTaitBryanAngles<float, ScenePos, 3>& TrackElementExtended::transformation() const {
    return element.transformation();
}

void TrackElementExtended::set_y_position(ScenePos value) {
    if (element.transformations.empty()) {
        throw std::runtime_error("Extended track element is empty");
    }
    auto diff = value - element.transformations.front().position(1);
    for (auto& t : element.transformations) {
        t.position(1) += diff;
    }

}

TrackElementExtended VA::interpolated(
    const TrackElementExtended& a,
    const TrackElementExtended& b,
    float alpha)
{
    return {
        .element = interpolated(a.element, b.element, alpha),
        .meters_to_start = (1 - alpha) * a.meters_to_start + alpha * b.meters_to_start
    };
}

std::ostream& VA::operator << (std::ostream& ostr, const TrackElementExtended& element) {
    ostr << element.element << " meters_to_start: " << element.meters_to_start;
    return ostr;
}
