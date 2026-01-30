// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Math/Math.hpp>
#include <stdexcept>

namespace VA {

template <class TDerived, class TData>
auto maximum(const BaseDenseArray<TDerived, TData>& a, const BaseDenseArray<TDerived, TData>& b) {
    return a->array_array_binop(*b, [](const TData& x, const TData& y) { return std::max(x, y); });
}

template <class TDerived, class TData>
auto maximum(const BaseDenseArray<TDerived, TData>& a, const TData& b) {
    return a->applied([&b](const TData& x) { return std::max(x, b); });
}

template <class TDerived, class TData>
auto maximum(const TData& a, const BaseDenseArray<TDerived, TData>& b) {
    return b->applied([&a](const TData& x) { return std::max(a, x); });
}

template <class TDerived, class TData>
auto minimum(const BaseDenseArray<TDerived, TData>& a, const BaseDenseArray<TDerived, TData>& b) {
    return a->array_array_binop(*b, [](const TData& x, const TData& y) { return std::min(x, y); });
}

template <class TDerived, class TData>
auto minimum(const BaseDenseArray<TDerived, TData>& a, const TData& b) {
    return a->applied([&b](const TData& x) { return std::min(x, b); });
}

template <class TDerived, class TData>
auto minimum(const TData& a, const BaseDenseArray<TDerived, TData>& b) {
    return b->applied([&a](const TData& x) { return std::min(a, x); });
}

template <class TData>
Array<TData> max(const Array<TData>& x, size_t axis) {
    return x.apply_over_axis(axis, ApplyOverAxisType::REDUCE,
        [&](size_t i, size_t k, const Array<TData>& xf, Array<TData>& rf)
        {
            rf(i, k) = -INFINITY;
            for (size_t h = 0; h < x.shape(axis); ++h) {
                rf(i, k) = std::max(rf(i, k), xf(i, h, k));
            }
        });
}

template <class TData>
Array<TData> min(const Array<TData>& x, size_t axis) {
    return x.apply_over_axis(axis, ApplyOverAxisType::REDUCE,
        [&](size_t i, size_t k, const Array<TData>& xf, Array<TData>& rf)
        {
            rf(i, k) = INFINITY;
            for (size_t h = 0; h < x.shape(axis); ++h) {
                rf(i, k) = std::min(rf(i, k), xf(i, h, k));
            }
        });
}

template <class TDerived, class TData>
TData max(const BaseDenseArray<TDerived, TData>& a) {
    auto f = a->flat_iterable();
    if (f.begin() == f.end()) {
        throw std::runtime_error("Cannot determine maximum of array of length 0");
    }
    TData result = *f.begin();
    for (auto it = f.begin() + 1; it != f.end(); ++it) {
        result = std::max(result, *it);
    }
    return result;
}

template <class TDerived, class TData>
TData min(const BaseDenseArray<TDerived, TData>& a) {
    auto f = a->flat_iterable();
    if (f.begin() == f.end()) {
        throw std::runtime_error("Cannot determine minimum of array of length 0");
    }
    TData result = *f.begin();
    for (auto it = f.begin() + 1; it != f.end(); ++it) {
        result = std::min(result, *it);
    }
    return result;
}

template <class TData>
TData nanmax(const Array<TData>& a) {
    return max(a[!VA::isnan(a)]);
}

template <class TData>
TData nanmin(const Array<TData>& a) {
    return min(a[!VA::isnan(a)]);
}

}
