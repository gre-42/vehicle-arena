// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Array/Fixed_Array.hpp>
#include <VehicleArena/Type_Traits/Higher_Precision.hpp>

namespace VA {

template <class TData>
FixedArray<TData, 3, 3> cross(const FixedArray<TData, 3>& k) {
    return FixedArray<TData, 3, 3>::init(
        TData{0}, -k(2), k(1),
        k(2), TData{0}, -k(0),
        -k(1), k(0), TData{0});
}

template <class TData>
FixedArray<TData, 3> cross(const FixedArray<TData, 3>& a, const FixedArray<TData, 3>& b) {
    using H = typename HigherPrecision<TData>::value_type;
    using L = TData;
    return FixedArray<TData, 3>::init(
        L(H(-a(2)) * H(b(1)) + H(a(1)) * H(b(2))),
        L(H(a(2)) * H(b(0)) + H(-a(0)) * H(b(2))),
        L(H(-a(1)) * H(b(0)) + H(a(0)) * H(b(1))));
}

}
