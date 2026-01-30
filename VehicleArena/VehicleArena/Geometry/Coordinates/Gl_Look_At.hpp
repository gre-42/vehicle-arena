// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Geometry/Fixed_Cross.hpp>
#include <VehicleArena/Math/Fixed_Math.hpp>

namespace VA {

template <class TData>
std::optional<FixedArray<TData, 3, 3>> gl_lookat_relative(
    const FixedArray<TData, 3>& dz,
    const FixedArray<TData, 3>& dy0 = { 0.f, 1.f, 0.f })
{
    auto dx = cross(dy0, -dz);
    auto dx_len2 = sum(squared(dx));
    if (dx_len2 < 1e-12) {
        return std::nullopt;
    }
    dx /= std::sqrt(dx_len2);
    FixedArray<TData, 3> dy = cross(-dz, dx);
    return FixedArray<TData, 3, 3>::init(
        dx(0), dy(0), -dz(0),
        dx(1), dy(1), -dz(1),
        dx(2), dy(2), -dz(2));
}

template <class TData>
std::optional<FixedArray<TData, 3, 3>> gl_lookat_absolute(
    const FixedArray<TData, 3>& camera_pos,
    const FixedArray<TData, 3>& object_pos,
    const FixedArray<TData, 3>& dy0 = { 0.f, 1.f, 0.f })
{
    auto dz = object_pos - camera_pos;
    auto dz_len2 = sum(squared(dz));
    if (dz_len2 < 1e-12) {
        return std::nullopt;
    }
    return gl_lookat_relative(dz / std::sqrt(dz_len2), dy0);
}

}
