// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Array/Array.hpp>
#include <set>

namespace VA {

template <class TData>
struct PolyDescriptor {
    std::list<size_t> indices;
    Array<TData> data;
};

template <class TData>
Array<Array<TData>> vanderNd(const Array<Array<TData>>& x, size_t degree) {
    assert(x.ndim() == 1);
    // degree, combination
    std::vector<std::vector<PolyDescriptor<TData>>> v(degree + 1);
    v[0].push_back(PolyDescriptor<TData>{
        .indices = {},
        .data = ones<TData>(x(0).shape())});
    size_t nelems = 1;
    for (size_t d = 1; d <= degree; ++d) {
        v[d].reserve(x.shape(0) * v[d-1].size());
        std::set<std::list<size_t>> inserted_indices;
        for (const auto& vv : v[d-1]) {
            for (size_t xid = 0; xid < x.length(); ++xid) {
                std::list<size_t> new_indices = vv.indices;
                new_indices.push_back(xid);
                new_indices.sort();
                if (!inserted_indices.contains(new_indices)) {
                    inserted_indices.insert(new_indices);
                    v[d].push_back(PolyDescriptor<TData>{
                        .indices = new_indices,
                        .data = x(xid) * vv.data});
                    ++nelems;
                }
            }
        }
    }
    size_t i = 0;
    Array<Array<TData>> result{ArrayShape{nelems}};
    for (size_t d = 0; d <= degree; ++d) {
        for (const auto& vv : v[d]) {
            result(i) = vv.data;
            ++i;
        }
    }
    return result;
}

}
