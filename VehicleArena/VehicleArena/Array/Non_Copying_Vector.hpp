// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Throw_Or_Abort.hpp>
#include <cassert>
#include <cstddef>

namespace VA {

template <class T>
struct alignas(T) ObjectBlob {
    std::byte data[sizeof(T)];
};

/*
 * std::vector replacement to circumvent <bool> template specialization
 */
template <class T>
class NonCopyingVector {
    NonCopyingVector(const NonCopyingVector&) = delete;
    NonCopyingVector& operator = (const NonCopyingVector&) = delete;
    ObjectBlob<T>* data_;
    size_t capacity_;
    size_t size_;
public:
    inline explicit NonCopyingVector(size_t capacity)
        : data_{ new ObjectBlob<T>[capacity] }
        , capacity_{ capacity }
        , size_{ 0 }
    {}
    inline ~NonCopyingVector() {
        clear();
        delete [] data_;
    }
    void clear() {
        while (size_ > 0) {
            (*this)[size_ - 1].~T();
            --size_;
        }
    }
    void clear_and_reserve(size_t capacity) {
        clear();
        delete [] data_;
        data_ = new ObjectBlob<T>[capacity];
        capacity_ = capacity;
    }
    template <class... Args>
    inline T& emplace_back(Args... args) {
        return *new(data_[size_++].data) T(std::forward<Args>(args)...);
    }
    inline const T& operator [] (size_t index) const {
        assert(index < size_);
        return reinterpret_cast<T&>(data_[index]);
    }
    inline T& operator [] (size_t index) {
        const NonCopyingVector& a = *this;
        return const_cast<T&>(a[index]);
    }
    inline const T& at(size_t index) const {
        if (index >= size_) {
            THROW_OR_ABORT("Index out of bounds");
        }
        return (*this)[index];
    }
    inline T& at(size_t index) {
        const auto& a = *this;
        return const_cast<T&>(a.at(index));
    }
    inline size_t size() const {
        return size_;
    }
    inline bool empty() const {
        return size_ == 0;
    }
    T* begin() {
        return empty() ? nullptr : &(*this)[0];
    }
    T* end() {
        return empty() ? nullptr : &(*this)[size_];
    }
};

}
