// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include "Array_Forward.hpp"
#include <VehicleArena/Math/Conju.hpp>

namespace VA {

template <class TData>
class ConjugateTransposeArray: public BaseDenseArray<ConjugateTransposeArray<TData>, TData> {
public:
    explicit ConjugateTransposeArray(const Array<TData>& a)
    : a_{a},
      shape_{a.shape(1), a.shape(0)}
    {
        assert(a.ndim() == 2);
    }
    const ArrayShape& shape() const {
        return shape_;
    }
    size_t shape(size_t i) const {
        return shape_(i);
    }
    template <size_t N>
    size_t static_shape() const {
        return shape(N);
    }
    size_t ndim() const {
        return 2;
    }
    TData operator () (size_t r, size_t c) const {
        return conju(a_(c, r));
    }
private:
    const Array<TData>& a_;
    const ArrayShape shape_;
};

}
