// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Math/Fixed_Math.hpp>
#include <VehicleArena/Os/Os.hpp>
#include <algorithm>
#include <stdexcept>

namespace VA {

template <class TData, size_t max_length>
class FixedHistory {
public:
    FixedHistory()
        : data(uninitialized)
        , next_{ 0 }
        , length_{ 0 }
    {}
    void clear() {
        static_assert(max_length > 0);
        next_ = 0;
        length_ = 0;
    }
    void append(const TData& value) {
        static_assert(max_length > 0);
        data(next_) = value;
        next_ = (next_ + 1) % max_length;
        length_ = std::min(max_length, length_ + 1);
    }
    size_t length() const {
        return length_;
    }
    bool empty() const {
        return (length_ == 0);
    }
    FixedArray<TData, max_length> data;
private:
    size_t next_;
    size_t length_;
};

template <class TData, size_t length>
TData max(const FixedHistory<TData, length>& b) {
    if (b.empty()) {
        throw std::runtime_error("Cannot compute maximum of empty history");
    }
    TData result = b.data(0);
    for (size_t i = 1; i < b.length(); ++i) {
        result = std::max(result, b.data(i));
    }
    return result;
}

}
