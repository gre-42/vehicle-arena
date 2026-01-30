// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Geometry/Exceptions/Triangle_Exception.hpp>
#include <VehicleArena/Geometry/Triangle_Tangent_Error_Behavior.hpp>
#include <VehicleArena/Math/Fixed_Math.hpp>

namespace VA {

/**
 * From: https://learnopengl.com/Advanced-Lighting/Normal-Mapping
 */
template <class TData>
FixedArray<TData, 3> triangle_tangent(
    const FixedArray<TData, 3>& pos0,
    const FixedArray<TData, 3>& pos1,
    const FixedArray<TData, 3>& pos2,
    const FixedArray<TData, 2>& uv0,
    const FixedArray<TData, 2>& uv1,
    const FixedArray<TData, 2>& uv2,
    TriangleTangentErrorBehavior error_behavior = TriangleTangentErrorBehavior::THROW)
{
    FixedArray<TData, 3> edge0 = pos1 - pos0;
    FixedArray<TData, 3> edge1 = pos2 - pos0;
    FixedArray<TData, 2> deltaUV0 = uv1 - uv0;
    FixedArray<TData, 2> deltaUV1 = uv2 - uv0;

    TData a = deltaUV0(0) * deltaUV1(1) - deltaUV1(0) * deltaUV0(1);
    if (std::abs(a) < 1e-12) {
        if (error_behavior == TriangleTangentErrorBehavior::ZERO) {
            return FixedArray<TData, 3>{TData{0}, TData{0}, TData{0}};
        } else if (error_behavior == TriangleTangentErrorBehavior::WARN) {
            lerr() << "Cannot calculate triangle tangent (0)";
            return FixedArray<TData, 3>{TData{0}, TData{0}, TData{0}};
        } else {
            throw TriangleException(pos0, pos1, pos2, "Cannot calculate triangle tangent (0)");
        }
    }
    TData f = TData{1} / a;

    TData ma = std::max(max(abs(edge0)), max(abs(edge1)));
    if (ma < 1e-12) {
        if (error_behavior == TriangleTangentErrorBehavior::ZERO) {
            return FixedArray<TData, 3>{TData{0}, TData{0}, TData{0}};
        } else if (error_behavior == TriangleTangentErrorBehavior::WARN) {
            lerr() << "Cannot calculate triangle tangent (1)";
            return FixedArray<TData, 3>{TData{0}, TData{0}, TData{0}};
        } else {
            throw TriangleException(pos0, pos1, pos2, "Cannot calculate triangle tangent (1)");
        }
    }
    edge0 /= ma;
    edge1 /= ma;
    FixedArray<TData, 3> tangent1 = f * (deltaUV1(1) * edge0 - deltaUV0(1) * edge1);
    TData len2 = sum(squared(tangent1));
    if (len2 < 1e-12) {
        if (error_behavior == TriangleTangentErrorBehavior::ZERO) {
            return FixedArray<TData, 3>{TData{0}, TData{0}, TData{0}};
        } else if (error_behavior == TriangleTangentErrorBehavior::WARN) {
            lerr() << "Cannot calculate triangle tangent (2)";
            return FixedArray<TData, 3>{TData{0}, TData{0}, TData{0}};
        } else {
            throw TriangleException(pos0, pos1, pos2, "Cannot calculate triangle tangent (2)");
        }
    }
    tangent1 /= std::sqrt(len2);
    return tangent1;
}

}
