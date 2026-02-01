// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Images/Filters/Central_Differences.hpp>
#include <VehicleArena/Math/Math.hpp>

namespace VA {

// From: https://stackoverflow.com/questions/5281261/generating-a-normal-map-from-a-height-map
template <class T>
Array<T> heightmap_to_normalmap(const Array<T>& heightmap, const T& d) {
    if (heightmap.ndim() != 2) {
        throw std::runtime_error("Heightmap is not 2D");
    }
    auto normalmap = Array<T>(ArrayShape{3, heightmap.shape(0), heightmap.shape(1)});
    normalmap.row_range(0, 2) = -central_gradient_filter(heightmap) * (T(1) / (2 * d));
    normalmap[2] = T(1);
    auto fac = T(1) / sqrt(sum(squared(normalmap), 0));
    for (size_t i = 0; i < 3; ++i) {
        normalmap[i] *= fac;
    }
    return normalmap;
}

}
