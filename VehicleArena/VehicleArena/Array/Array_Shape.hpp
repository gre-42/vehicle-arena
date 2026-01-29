// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include "Array_Forward.hpp"
#include <cassert>
#include <cstdint>
#include <iosfwd>
#include <string>
#include <vector>

namespace VA {

std::ostream &operator << (std::ostream &ostream, const ArrayShape &v);

class ArrayShape {
    std::vector<size_t> shape_;
    using iter_diff_type = std::vector<size_t>::iterator::difference_type;
public:
    template <class Archive>
    void serialize(Archive& archive) {
        archive(shape_);
    }

    ArrayShape();
    ArrayShape(const ArrayShape& shape);
    ArrayShape(ArrayShape&& shape);
    ArrayShape& operator = (const ArrayShape& shape);
    ArrayShape& operator = (ArrayShape&& shape);
    explicit ArrayShape(std::initializer_list<size_t> shape);
    explicit ArrayShape(size_t ndim);
    ArrayShape& operator = (size_t value);
    void append(size_t size);
    ArrayShape appended(size_t size) const;
    ArrayShape erased_first(size_t n = 1) const;
    ArrayShape erased_last(size_t n = 1) const;
    void clear();
    ArrayShape reverted() const;
    void concatenate(const ArrayShape& other);
    ArrayShape concatenated(const ArrayShape& other) const;
    const size_t& operator () (size_t i) const;
    size_t& operator () (size_t i);
    template <size_t N>
    const size_t& get() const {
        return (*this)(N);
    }
    size_t ndim() const;
    size_t nelements() const;
    size_t length() const;
    template <class TCallable>
    void foreach(
        const TCallable& f,
        const ArrayShape& prepend=ArrayShape()) const
    {
        if (ndim() == 0) {
            f(prepend);
        } else {
            // original code
            // for (size_t i=0; i<(*this)(0); i++) {
            //    erased_first().foreach(f, prepend.appended(i));
            //}
            // optimized code
            ArrayShape ef = erased_first();
            ArrayShape id = prepend.appended(SIZE_MAX);
            size_t last_id = id.ndim() - 1;
            for (size_t i = 0; i < (*this)(0); i++) {
                id(last_id) = i;
                ef.foreach(f, id);
            }
        }
    }
    template <class TCallable>
    void apply_over_axis(
        size_t axis,
        const TCallable& f) const
    {
        assert(axis < ndim());
        ArrayShape shape;
        shape = *this;
        shape(axis) = 1;
        shape.foreach([&](const ArrayShape& index0){
            f(index0);
        });
    }
    std::string str() const;
    size_t* begin();
    const size_t* begin() const;
};

template <class TBinop>
inline ArrayShape arrayshape_arrayshape_binop(const ArrayShape& a, const ArrayShape& b, const TBinop& binop) {
    assert(a.ndim() == b.ndim());
    ArrayShape result(a.ndim());
    for (size_t i = 0; i < a.ndim(); i++) {
        result(i) = binop(a(i), b(i));
    }
    return result;
}

template <class TOperation>
inline ArrayShape arrayshape_apply(const ArrayShape& a, const TOperation& operation) {
    ArrayShape result(a.ndim());
    for (size_t i = 0; i < a.ndim(); i++) {
        result(i) = operation(a(i));
    }
    return result;
}

inline ArrayShape operator + (const ArrayShape& a, const ArrayShape& b) {
    return arrayshape_arrayshape_binop(a, b, [](size_t x, size_t y){ return x + y; });
}

inline ArrayShape operator - (const ArrayShape& a, const ArrayShape& b) {
    return arrayshape_arrayshape_binop(a, b, [](size_t x, size_t y){ return x - y; });
}

inline ArrayShape operator / (const ArrayShape& a, const ArrayShape& b) {
    return arrayshape_arrayshape_binop(a, b, [&](size_t x, size_t y){ return x / y; });
}

inline ArrayShape operator % (const ArrayShape& a, const ArrayShape& b) {
    return arrayshape_arrayshape_binop(a, b, [&](size_t x, size_t y){ return x % y; });
}

inline ArrayShape operator + (const ArrayShape& a, size_t b) {
    return arrayshape_apply(a, [&](size_t x){ return x + b; });
}

inline ArrayShape operator - (const ArrayShape& a, size_t b) {
    return arrayshape_apply(a, [&](size_t x){ return x - b; });
}

inline ArrayShape operator * (const ArrayShape& a, size_t b) {
    return arrayshape_apply(a, [&](size_t x){ return x * b; });
}

inline ArrayShape operator * (size_t a, const ArrayShape& b) {
    return b * a;
}

inline ArrayShape operator / (const ArrayShape& a, size_t b) {
    return arrayshape_apply(a, [&](size_t x){ return x / b; });
}

inline ArrayShape operator % (const ArrayShape& a, size_t b) {
    return arrayshape_apply(a, [&](size_t x){ return x % b; });
}

inline ArrayShape operator & (const ArrayShape& a, size_t b) {
    return arrayshape_apply(a, [&](size_t x){ return x & b; });
}

inline ArrayShape operator | (const ArrayShape& a, size_t b) {
    return arrayshape_apply(a, [&](size_t x){ return x | b; });
}

}
