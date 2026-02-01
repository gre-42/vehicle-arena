// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Physics/Misc/Track_Element.hpp>

namespace VA {

enum class TrackElementInterpolationKey {
    ELAPSED_SECONDS,
    METERS_TO_START
};

struct TrackElementExtended {
    TrackElement element;
    double meters_to_start;
    static TrackElementExtended create(
        const std::optional<TrackElementExtended>& predecessor,
        const TrackElement& track_element);
    inline double progress(TrackElementInterpolationKey key) const {
        if (key == TrackElementInterpolationKey::ELAPSED_SECONDS) {
            if (std::isnan(element.elapsed_seconds)) {
                throw std::runtime_error("Elapsed seconds is NAN despite selected interpolation key");
            }
            return element.elapsed_seconds;
        }
        if (key == TrackElementInterpolationKey::METERS_TO_START) {
            return meters_to_start;
        }
        throw std::runtime_error("Unknown track element interpolation key");
    }
    const OffsetAndTaitBryanAngles<float, ScenePos, 3>& transformation() const;
    void set_y_position(ScenePos value);
};

TrackElementExtended interpolated(const TrackElementExtended& a, const TrackElementExtended& b, float alpha);

std::ostream& operator << (std::ostream& ostr, const TrackElementExtended& element);

}
