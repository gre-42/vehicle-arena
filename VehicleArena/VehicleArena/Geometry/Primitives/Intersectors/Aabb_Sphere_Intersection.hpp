// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Geometry/Primitives/Axis_Aligned_Bounding_Box.hpp>
#include <VehicleArena/Geometry/Primitives/Bounding_Sphere.hpp>

#ifdef __GNUC__
    #pragma GCC push_options
    #pragma GCC optimize ("O3")
#endif

namespace VA {

template <class TData, size_t tndim>
bool aabb_intersects_sphere(
    const AxisAlignedBoundingBox<TData, tndim>& aabb,
    const BoundingSphere<TData, tndim>& sphere)
{
    auto distance1 = abs(aabb.closest_point(sphere.center) - sphere.center);
    if (any(distance1 > sphere.radius)) {
        return false;
    }
    auto distance2 = sum(squared(funpack(distance1)));
    return distance2 <= squared(funpack(sphere.radius));
}

}

#ifdef __GNUC__
    #pragma GCC pop_options
#endif
