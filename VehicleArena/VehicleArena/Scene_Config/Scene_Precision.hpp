// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Hashing/Hash.hpp>
#include <VehicleArena/Hashing/Std_Hash.hpp>
#include <VehicleArena/Math/Fixed_Point_Number.hpp>

namespace VA {

using SceneDir = float;
using ScenePos = double;

static const std::intmax_t SCENE_POS_DENOMINATOR = (1 << 11);
using CompressedScenePos = FixedPointNumber<int32_t, SCENE_POS_DENOMINATOR>;
using HalfCompressedScenePos = FixedPointNumber<int16_t, SCENE_POS_DENOMINATOR>;

}

template <>
struct std::hash<VA::CompressedScenePos>
{
    std::size_t operator() (const VA::CompressedScenePos& a) const {
        return VA::hash_combine(a.count);
    }
};
