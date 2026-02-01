// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Array/Fixed_Array.hpp>
#include <VehicleArena/Initialization/Default_Uninitialized_Vector.hpp>
#include <VehicleArena/Math/Transformation/Tait_Bryan_Angles.hpp>
#include <VehicleArena/Scene_Config/Scene_Precision.hpp>
#include <iosfwd>
#include <vector>

namespace VA {

template <class TDir, class TPos, size_t n>
class TransformationMatrix;
template <class TDir, class TPos, size_t n>
class OffsetAndTaitBryanAngles;

struct TrackElement {
    void write_to_stream(
        std::ostream& ostr,
        const TransformationMatrix<double, double, 3>& geographic_mapping) const;
    std::vector<double> to_vector(
        const TransformationMatrix<double, double, 3>& geographic_mapping) const;
    static TrackElement from_stream(
        std::istream& istr,
        const TransformationMatrix<double, double, 3>& geographic_mapping,
        size_t ntransformations);
    static TrackElement from_vector(
        const std::vector<double>& data,
        const TransformationMatrix<double, double, 3>& geographic_mapping,
        size_t ntransformations);
    const OffsetAndTaitBryanAngles<float, ScenePos, 3>& transformation() const;

    float elapsed_seconds;
    UVector<OffsetAndTaitBryanAngles<float, ScenePos, 3>> transformations;
};

TrackElement interpolated(const TrackElement& a, const TrackElement& b, float alpha);

std::ostream& operator << (std::ostream& ostr, const TrackElement& element);

}
