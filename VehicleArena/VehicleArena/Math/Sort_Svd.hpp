// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Array/Array.hpp>
#include <VehicleArena/Stats/Sort.hpp>

namespace VA {

template <class TData>
void sort_svd(Array<TData>& u, Array<TData>& s, Array<TData>& vT)
{
    Array<size_t> ids = argsort(s, SortingDirection::descending);

    Array<TData> pu;
    Array<TData> ps;
    Array<TData> pvT;

    pu.do_resize(u.shape());
    ps.do_resize(s.shape());
    pvT.do_resize(vT.shape());
    for (size_t i = 0; i < s.length(); ++i) {
        ps(i) = s(ids(i));
    }
    for (size_t r = 0; r < u.shape(0); ++r) {
        for (size_t c = 0; c < u.shape(1); ++c) {
            // preserves order for singular values == 0
            pu(r, c) = u(r, c < ids.length() ? ids(c) : c);
        }
    }
    for (size_t r = 0; r < vT.shape(0); ++r) {
        for (size_t c = 0; c < vT.shape(1); ++c) {
            pvT(r, c) = vT(ids(r), c);
        }
    }
    u = pu;
    s = ps;
    vT = pvT;
}

};
