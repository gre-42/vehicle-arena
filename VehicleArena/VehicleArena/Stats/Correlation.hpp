// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Stats/Mean.hpp>

namespace VA {

template <class TDerivedA, class TDerivedB, class TData>
TData correlation(
    const BaseDenseArray<TDerivedA, TData>& a,
    const BaseDenseArray<TDerivedB, TData>& b,
    const TData& variance_threshold = 0)
{
    auto ma = *a - mean(*a);
    auto mb = *b - mean(*b);
    TData va = sum(squared(ma)) / (a->length() - 1);
    TData vb = sum(squared(mb)) / (b->length() - 1);
    if (std::abs(va) < variance_threshold || std::abs(vb) < variance_threshold) {
        return TData(0);
    }
    return dot0d(ma, mb) / std::sqrt(va * vb);
}

template <class TDerivedA, class TDerivedB, class TData>
TData normalized_difference(
    const BaseDenseArray<TDerivedA, TData>& a,
    const BaseDenseArray<TDerivedB, TData>& b,
    const TData& variance_threshold = 0)
{
    TData da = sum(squared(*a));
    TData db = sum(squared(*b));
    if (std::abs(da) < variance_threshold || std::abs(db) < variance_threshold) {
        return TData(0);
    }
    return sum(squared(*a - *b)) / std::sqrt(da * db);
}

}
