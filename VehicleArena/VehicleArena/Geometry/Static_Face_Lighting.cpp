// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Static_Face_Lighting.hpp"
#include <VehicleArena/Geometry/Triangle_Normal.hpp>

using namespace VA;

StaticFaceLighting::StaticFaceLighting(bool swap_yz)
    : light_direction{0.2f, 0.8f, 0.4f}
    , diffuse_color{0.6f, 0.6f, 0.6f}
    , ambient_color{0.5f, 0.5f, 0.5f}
{
    light_direction /= std::sqrt(sum(squared(light_direction)));
    if (swap_yz) {
        std::swap(light_direction(1), light_direction(2));
    }
}

FixedArray<float, 3> StaticFaceLighting::get_color(
    const FixedArray<float, 3>& reflectance,
    const FixedArray<float, 3>& normal) const
{
    return reflectance * (
        ambient_color +
        diffuse_color * std::pow(std::abs(dot0d(normal, light_direction)), 2.f));
}
