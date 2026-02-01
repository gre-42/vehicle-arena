// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Math/Fixed_Math.hpp>
#include <VehicleArena/Math/Orderable_Fixed_Array.hpp>
#include <cmath>
#include <compare>
#include <stdexcept>

namespace VA {

template <typename TData, size_t... tshape>
class FixedArray;

template <class TPos>
class VertexHeightBinding {
    static constexpr const TPos UNDEFINED = std::numeric_limits<TPos>::max();
public:
    VertexHeightBinding()
        : value_{ UNDEFINED, UNDEFINED }
    {}
    VertexHeightBinding& operator = (const FixedArray<TPos, 2>& v) {
        if (!is_undefined()) {
            throw std::runtime_error("Height binding already set");
        }
        if (any(v == UNDEFINED)) {
            throw std::runtime_error("Height binding value forbidden");
        }
        value_ = v;
        return *this;
    }
    std::strong_ordering operator <=> (const FixedArray<TPos, 2>& v) const {
        if (is_undefined()) {
            throw std::runtime_error("Height binding undefined");
        }
        return make_orderable(value_) <=> make_orderable(v);
    }
    const FixedArray<TPos, 2>& value() const {
        return value_;
    }
    FixedArray<TPos, 2>& value() {
        return value_;
    }
private:
    bool is_undefined() const {
        return all(value_ == UNDEFINED);
    }
    FixedArray<TPos, 2> value_;
};

}
