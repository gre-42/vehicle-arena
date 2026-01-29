// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include "Array_Forward.hpp"
#include <cassert>

namespace VA {

template <class TData>
class ArrayAxisView {
    Array<TData> array_;
    size_t offset_;
    size_t stride_;
    size_t length_;
public:
    ArrayAxisView(const Array<TData>& array, const ArrayShape& index0, size_t axis):
        array_(array)
    {
        assert(index0(axis) == 0);
        ArrayShape index1(index0);
        index1(axis) = 1;
        assert(array_.data_ != nullptr);
        assert(array_.data_->size() > 0);
        offset_ = (size_t)(&array_(index0) - &(*array_.data_)[0]);
        if (array_.shape(axis) > 1) {
            stride_ = (size_t)(&array_(index1) - &array_(index0));
        } else {
            stride_ = SIZE_MAX;
        }
        length_ = array_.shape(axis);
    }
    const TData& operator () (size_t index) const {
        size_t i = offset_ + stride_ * index;
        assert(index < length());
        assert(i < array_.data_->size());
        return (*array_.data_)[i];
    }
    TData& operator () (size_t index) {
        const ArrayAxisView& a = *this;
        return const_cast<TData&>(a(index));
    }
    size_t length() const {
        return length_;
    }
};

}
