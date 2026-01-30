// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Array/Array.hpp>
#include <cmath>
#include <cstddef>

namespace VA {

static const size_t id1 = 1;
static const size_t id0 = 0;

inline size_t a2i(float a) {
    return size_t(std::lround(std::floor(a)));
}

inline size_t fi2i(float fi) {
    return size_t(std::lround(std::round(fi)));
}

inline float i2fi(size_t i) {
    return float(i);
}

inline ArrayShape a2i(const Array<float>& a) {
    assert(all(a.shape() == ArrayShape{2}));
    return ArrayShape{
        a2i(a(id1)),
        a2i(a(id0))};
}

inline float i2a(size_t i) {
    // Adding 0.5 selects the center of the pixel
    return float(i) + 0.5f;
}

inline Array<float> i2a(const ArrayShape& i) {
    assert(i.ndim() == 2);
    return Array<float>{
        i2a(i(id1)),
        i2a(i(id0))};
}

inline float a2fi(float a) {
    return a - 0.5f;
}

inline float fi2a(float fi) {
    return fi + 0.5f;
}

inline Array<float> a2fi(const Array<float>& a) {
    assert(all(a.shape() == ArrayShape{2}));
    return Array<float>{
        a2fi(a(id1)),
        a2fi(a(id0))};
}

}
