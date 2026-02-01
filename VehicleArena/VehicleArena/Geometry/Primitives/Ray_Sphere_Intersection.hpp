// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Math/Fixed_Math.hpp>

namespace VA {

template <class TData, size_t tndim>
bool ray_intersects_sphere(
    const FixedArray<TData, tndim>& ray_origin,
    const FixedArray<TData, tndim>& ray_direction,
    const FixedArray<TData, tndim>& sphere_center,
    TData radius_squared,
    TData* lambda0,
    TData* lambda1)
{
    auto R = ray_origin - sphere_center;
    auto b = 2 * dot0d(R, ray_direction);
    auto c = sum(squared(R)) - radius_squared;
    auto s = squared(b) - 4 * c;
    if (s < 0) {
        return false;
    }
    auto q = std::sqrt(s);
    if (lambda0 != nullptr) {
        *lambda0 = (-b + q) * TData(0.5);
    }
    if (lambda1 != nullptr) {
        *lambda1 = (-b - q) * TData(0.5);
    }
    return true;
}

}
