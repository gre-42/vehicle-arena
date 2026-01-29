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
class TranslationMatrix {
public:
    explicit TranslationMatrix(const FixedArray<T, n>& t)
        : t(t)
    {}
    FixedArray<T, n> t;
};

template <class TData, size_t n>
class LeftArray: public BaseDenseFixedArray<LeftArray<TData, n>, TData, n+1, n> {
public:
    LeftArray(const FixedArray<TData, n+1, n+1>& a)
        : a_{ a }
    {}
    const TData& operator () (size_t r, size_t c) const {
        return a_(r, c);
    }
private:
    const FixedArray<TData, n+1, n+1>& a_;
};

template <class T, size_t n>
FixedArray<T, n+1, n+1> operator * (const FixedArray<T, n+1, n+1>& a, const TranslationMatrix<T, n>& b) {
    auto result = a;
    auto Rb = dot1d(LeftArray<T, n>{a}, b.t);
    for (size_t i = 0; i <= n; ++i) {
        result(i, n) += Rb(i);
    }
    return result;
}

template <class TDir, class TPos, size_t n>
TransformationMatrix<TDir, TPos, n> operator * (const TransformationMatrix<TDir, TPos, n>& a, const TranslationMatrix<TPos, n>& b) {
    return TransformationMatrix<TDir, TPos, n>{a.R, a.transform(b.t)};
}

}
