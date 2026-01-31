// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Bone.hpp"
#include <VehicleArena/Math/Fixed_Math.hpp>
#include <stdexcept>

using namespace VA;

UUVector<OffsetAndQuaternion<float, float>> Bone::rebase_to_initial_absolute_transform(
    const UUVector<OffsetAndQuaternion<float, float>>& transformations)
{
    UUVector<OffsetAndQuaternion<float, float>> result;
    result.resize(transformations.size());
#ifndef NDEBUG
    for (OffsetAndQuaternion<float, float>& r : result) {
        r.t = NAN;
    }
#endif
    rebase_to_initial_absolute_transform(
        transformations,
        OffsetAndQuaternion<float, float>::identity(),
        result);
#ifndef NDEBUG
    for (const OffsetAndQuaternion<float, float>& r : result) {
        if (any(VA::isnan(r.t))) {
            throw std::runtime_error("Bone transformation contains NAN values");
        }
    }
#endif
    return result;
}

void Bone::rebase_to_initial_absolute_transform(
    const UUVector<OffsetAndQuaternion<float, float>>& transformations,
    const OffsetAndQuaternion<float, float>& parent_transformation,
    UUVector<OffsetAndQuaternion<float, float>>& result)
{
#ifndef NDEBUG
    if (index >= result.size()) {
        throw std::runtime_error("Bone index too large for result array");
    }
    if (index >= transformations.size()) {
        throw std::runtime_error("Bone index too large for transformations");
    }
#endif
    const OffsetAndQuaternion<float, float>& m = initial_absolute_transformation;
    OffsetAndQuaternion<float, float> n = parent_transformation * transformations[index];
    result[index] = n * m.inverse();
    for (const auto& c : children) {
        c->rebase_to_initial_absolute_transform(
            transformations,
            n,
            result);
    }
}
