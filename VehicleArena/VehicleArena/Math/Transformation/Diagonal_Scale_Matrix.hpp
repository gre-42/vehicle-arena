// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Math/Transformation/Transformation_Matrix.hpp>

namespace VA {

template <class T, size_t n>
class DiagonalScaleMatrix {
public:
    explicit DiagonalScaleMatrix(const FixedArray<T, n>& d)
        : d(d)
    {}
    FixedArray<T, n> d;
};

template <class T, size_t R, size_t C>
FixedArray<T, R, C> dot2d(const FixedArray<T, R, C>& lhs, const DiagonalScaleMatrix<T, C>& d) {
    FixedArray<T, R, C> result = uninitialized;
    for (size_t r = 0; r < R; ++r) {
        for (size_t c = 0; c < C; ++c) {
            result(r, c) = lhs(r, c) * d.d(c);
        }
    }
    return result;
}

template <class T, size_t R, size_t C>
FixedArray<T, R, C> dot2d(const DiagonalScaleMatrix<T, R>& d, const FixedArray<T, R, C>& rhs) {
    FixedArray<T, R, C> result = uninitialized;
    for (size_t r = 0; r < R; ++r) {
        for (size_t c = 0; c < C; ++c) {
            result(r, c) = rhs(r, c) * d.d(r);
        }
    }
    return result;
}

template <class TDir, class TPos, size_t n>
TransformationMatrix<TDir, TPos, n> operator * (const TransformationMatrix<TDir, TPos, n>& a, const DiagonalScaleMatrix<TDir, n>& b) {
    return TransformationMatrix<TDir, TPos, n>{dot2d(a.R, b), a.t};
}

}
