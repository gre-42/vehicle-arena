// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Geometry/Exceptions/Triangle_Exception.hpp>
#include <VehicleArena/Geometry/Fixed_Cross.hpp>
#include <VehicleArena/Geometry/Normal_Vector_Error_Behavior.hpp>
#include <VehicleArena/Math/Fixed_Math.hpp>
#include <VehicleArena/Os/Os.hpp>
#include <VehicleArena/Stats/Min_Max.hpp>
#include <optional>
#include <stdexcept>

namespace VA {

template <class TData>
FixedArray<TData, 3> scaled_triangle_normal(const FixedArray<TData, 3, 3>& t)
{
    return cross(t[2] - t[1], t[0] - t[1]);
}

template <class TData>
std::optional<FixedArray<TData, 3>> try_triangle_normal(
    const FixedArray<TData, 3, 3>& t)
{
    FixedArray<TData, 3> res = scaled_triangle_normal(t);
    TData ma = max(abs(res));
    if (ma < 1e-12) {
        return std::nullopt;
    }
    res /= ma;
    res /= std::sqrt(sum(squared(res)));
    return res;
}

template <class TData>
FixedArray<TData, 3> triangle_normal(
    const FixedArray<TData, 3, 3>& t,
    NormalVectorErrorBehavior error_behavior = NormalVectorErrorBehavior::THROW)
{
    auto res = try_triangle_normal(t);
    if (!res.has_value()) {
        return get_alternative_or_throw<TData>(t, error_behavior);
    }
    return *res;
}

template <class TNormal, class TTriangle>
FixedArray<TNormal, 3> get_alternative_or_throw(
    const FixedArray<TTriangle, 3, 3>& t,
    NormalVectorErrorBehavior error_behavior) {
    if (any(error_behavior & NormalVectorErrorBehavior::WARN)) {
        lwarn() << "Cannot calculate triangle normal";
    }
    if (any(error_behavior & NormalVectorErrorBehavior::THROW)) {
        throw TriangleException(t[0], t[1], t[2], "Cannot calculate triangle normal");
    }
    return fixed_zeros<TNormal, 3>();
}

}
