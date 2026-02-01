// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Math/Math.hpp>

namespace VA {

template <class TIn>
Array<uint8_t> meters_to_terrarium(const Array<TIn>& meters) {
    if (meters.ndim() != 2) {
        throw std::runtime_error("Meters is not a 2D image");
    }
    Array<uint8_t> rgb{ ArrayShape{3, meters.shape(0), meters.shape(1) }};
    // https://www.mapzen.com/blog/elevation/
    Array<TIn> val = meters + TIn(32768);
    for (size_t c = 2; c != SIZE_MAX; --c) {
        auto vc = val - val.applied([](float v){return std::floor(v);});
        rgb[c] = minimum(round(vc * TIn(256)), TIn(255)).template casted<uint8_t>();
        val -= vc;
        val /= 256;
    }
    return rgb;
}

template <class TOut, class TIn>
TOut terrarium_to_meters_pix(const TIn terrarium[3]) {
    return terrarium[0] * TOut(256) + terrarium[1] + terrarium[2] / TOut(256) - TOut(32768);
}

template <class TOut>
Array<TOut> terrarium_to_meters(const Array<uint8_t>& terrarium) {
    if (terrarium.ndim() != 3) {
        throw std::runtime_error("Terrarium heightmap does not have 3 dimensions");
    }
    if (terrarium.shape(0) != 3) {
        throw std::runtime_error("Terrarium heightmap does not have 3 channels");
    }
    return
        terrarium[0].template casted<TOut>() * TOut(256) +
        terrarium[1].template casted<TOut>() +
        terrarium[2].template casted<TOut>() / TOut(256) - TOut(32768);
}

}
