// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Initialization/Uninitialized.hpp>
#include <VehicleArena/Math/Funpack.hpp>
#include <concepts>
#include <ostream>
#include <utility>

namespace VA {

template <class TPosition, class TFlags>
struct PointAndFlags {
    using Point = TPosition;
    using value_type = TPosition::value_type;
    static consteval size_t length() {
        return TPosition::length();
    }
    PointAndFlags(Uninitialized)
        : position{ uninitialized }
    {}
    PointAndFlags(
        const TPosition& position,
        const TFlags& flags)
        : position{ position }
        , flags{ flags }
    {}
    operator const Point& () const {
        return position;
    }
    operator Point& () {
        return position;
    }
    PointAndFlags operator + (const PointAndFlags& other) const {
        return {
            position + other.position,
            flags | other.flags
        };
    }
    TPosition operator - (const PointAndFlags& other) const {
        return position - other.position;
    }
    PointAndFlags& operator |= (const PointAndFlags& other) {
        flags |= other.flags;
        return *this;
    }
    template <class Archive>
    void serialize(Archive& archive) {
        archive(position);
        archive(flags);
    }
    TPosition position;
    TFlags flags;
};

template <class T>
inline T gen_obj() {
    std::unreachable();
}

template <class TPosition, class TFlags, std::floating_point TRhs>
auto operator * (
    const PointAndFlags<TPosition, TFlags>& lhs,
    const TRhs& f)
{
    using V0 = typename TPosition::value_type;
    using V1 = funpack_t<V0>;
    using TResultV1 = decltype(V1() * TRhs());
    return PointAndFlags<TPosition, TFlags>{
        (lhs.position.template casted<TResultV1>() * (TResultV1)f).template casted<V0>(),
        lhs.flags
    };
}

template <std::floating_point TLhs, class TPosition, class TFlags>
auto operator * (
    const TLhs& f,
    const PointAndFlags<TPosition, TFlags>& rhs)
{
    using V = typename TPosition::value_type;
    using TResultV = decltype(TLhs() * V());
    using TResult = decltype(TPosition().template casted<TResultV>());
    return PointAndFlags<TResult, TFlags>{
        (TResultV)f * rhs.position.template casted<TResultV>(),
        rhs.flags
    };
}

template <class TPosition, class TFlags>
std::ostream& operator << (std::ostream& ostr, const PointAndFlags<TPosition, TFlags>& p) {
    return (ostr << '(' << p.position << ", " << p.flags << ')');
}

}
