// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Default_Uninitialized.hpp>

namespace VA {

/*
 * std::vector replacement to circumvent <bool> template specialization
 */
template <class TData>
class Vector {
    Vector(const Vector&) = delete;
    Vector& operator = (const Vector&) = delete;
    using UData = default_uninitialized_t<TData>;
    UData* data_;
    size_t size_;
public:
    inline explicit Vector(size_t size, Uninitialized):
        data_(new UData[size]),
        size_(size) {}
    inline ~Vector() {
        delete [] data_;
    }
    inline const TData& operator [] (size_t index) const {
        assert(index < size_);
        return data_[index];
    }
    inline TData& operator [] (size_t index) {
        const Vector& a = *this;
        return const_cast<TData&>(a[index]);
    }
    inline size_t size() const {
        return size_;
    }
};

}
