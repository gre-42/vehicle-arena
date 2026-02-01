// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Uv_Shifter.hpp"
#include <VehicleArena/Geometry/Material/Wrap_Mode.hpp>
#include <VehicleArena/Geometry/Texture/Uv_Atlas_Tolerance.hpp>

using namespace VA;

void VA::shift_uv3(
    float period,
    FixedArray<float, 2>& u0,
    FixedArray<float, 2>& u1,
    FixedArray<float, 2>& u2,
    WrapMode wrap_mode,
    size_t i)
{
    auto offset = (wrap_mode == WrapMode::REPEAT)
        ? std::round((u0(i) + u1(i) + u2(i)) / 3.f / period) * period
        : std::floor((std::min({u0(i), u1(i), u2(i)}) - UV_ATLAS_MIN) / period) * period;
    u0(i) -= offset;
    u1(i) -= offset;
    u2(i) -= offset;
}

template <class TPos>
UvShifter3<TPos>::UvShifter3(
    TPos period,
    const FixedArray<TPos, 2>& u0,
    const FixedArray<TPos, 2>& u1,
    const FixedArray<TPos, 2>& u2,
    const FixedArray<WrapMode, 2>& wrap_mode)
    : u0{ uninitialized }
    , u1{ uninitialized }
    , u2{ uninitialized }
{
    for (size_t i = 0; i < 2; ++i) {
        auto offset = (wrap_mode(i) == WrapMode::REPEAT)
            ? std::round((u0(i) + u1(i) + u2(i)) / TPos(3) / period) * period
            : std::floor((std::min({u0(i), u1(i), u2(i)}) - UV_ATLAS_MIN) / period) * period;
        this->u0(i) = float(u0(i) - offset);
        this->u1(i) = float(u1(i) - offset);
        this->u2(i) = float(u2(i) - offset);
    }
}

template <class TPos>
UvShifter4<TPos>::UvShifter4(
    TPos period,
    const FixedArray<TPos, 2>& u0,
    const FixedArray<TPos, 2>& u1,
    const FixedArray<TPos, 2>& u2,
    const FixedArray<TPos, 2>& u3,
    const FixedArray<WrapMode, 2>& wrap_mode)
    : u0{ uninitialized }
    , u1{ uninitialized }
    , u2{ uninitialized }
    , u3{ uninitialized }
{
    for (size_t i = 0; i < 2; ++i) {
        auto offset = (wrap_mode(i) == WrapMode::REPEAT)
            ? std::round((u0(i) + u1(i) + u2(i) + u3(i)) / TPos(4) / period) * period
            : std::floor((std::min({u0(i), u1(i), u2(i), u3(i)}) - UV_ATLAS_MIN) / period) * period;
        this->u0(i) = float(u0(i) - offset);
        this->u1(i) = float(u1(i) - offset);
        this->u2(i) = float(u2(i) - offset);
        this->u3(i) = float(u3(i) - offset);
    }
}

namespace VA {
    template struct UvShifter3<float>;
    template struct UvShifter3<double>;
    template struct UvShifter4<float>;
    template struct UvShifter4<double>;
}
