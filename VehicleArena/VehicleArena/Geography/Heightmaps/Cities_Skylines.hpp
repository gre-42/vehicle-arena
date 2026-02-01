// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Images/Normalize.hpp>
#include <VehicleArena/Math/Math.hpp>

namespace VA {

template <class TData>
inline Array<uint16_t> meters_to_cities_skylines(const Array<TData>& meters) {
    // https://www.mapzen.com/blog/elevation/
    return round(clipped(meters * TData(64), TData(0), TData(UINT16_MAX))).template casted<uint16_t>();
}

template <class TData>
inline Array<TData> cities_skylines_to_meters(const Array<uint16_t>& cities_skylines) {
    // https://www.mapzen.com/blog/elevation/
    return cities_skylines.casted<TData>() / TData(64);
}

}
