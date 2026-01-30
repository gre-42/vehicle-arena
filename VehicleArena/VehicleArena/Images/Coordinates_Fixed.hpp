// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include "Coordinates.hpp"
#include <VehicleArena/Array/Fixed_Array.hpp>

namespace VA {

inline FixedArray<float, 2> i2a(const FixedArray<size_t, 2>& i) {
    return FixedArray<float, 2>{
        i2a(i(id1)),
        i2a(i(id0))};
}

inline FixedArray<size_t, 2> a2i(const FixedArray<float, 2>& a) {
    return FixedArray<size_t, 2>{
        a2i(a(id1)),
        a2i(a(id0))};
}

inline FixedArray<float, 2> a2fi(const FixedArray<float, 2>& a) {
    return FixedArray<float, 2>{
        a2fi(a(id1)),
        a2fi(a(id0))};
}

inline FixedArray<float, 2> fi2a(const FixedArray<float, 2>& i) {
    return FixedArray<float, 2>{
        fi2a(i(id1)),
        fi2a(i(id0))};
}

// na: normalized a
inline FixedArray<float, 2> na2fi(const FixedArray<float, 2>& na) {
    return FixedArray<float, 2>{
        na(id1),
        na(id0)};
}

// na: normalized a
inline FixedArray<size_t, 2> na2i(const FixedArray<float, 2>& na) {
    return FixedArray<size_t, 2>{
        fi2i(na(id1)),
        fi2i(na(id0))};
}

}
