// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Geometry/Primitives/Bvh.hpp>
#include <VehicleArena/Geometry/Primitives/Collision_Line.hpp>
#include <VehicleArena/Scene_Config/Scene_Precision.hpp>

namespace VA {

class RigidBodyVehicle;

template <class TPosition>
struct RigidBodyAndCollisionLineSphere {
    RigidBodyVehicle& rb;
    CollisionLineSphere<TPosition> clp;
    template <class TPosition2>
    RigidBodyAndCollisionLineSphere<TPosition2> casted() const {
        return { rb, clp.template casted<TPosition2>() };
    }
    bool operator == (const RigidBodyAndCollisionLineSphere& other) const {
        return (&rb == &other.rb) &&
               (clp == other.clp);
    }
};

inline RigidBodyAndCollisionLineSphere<CompressedScenePos>
    operator + (
        const RigidBodyAndCollisionLineSphere<CompressedScenePos>& d,
        const FixedArray<CompressedScenePos, 3>& ref)
{
    return { d.rb, d.clp + ref };
}

inline RigidBodyAndCollisionLineSphere<CompressedScenePos>
    operator - (
        const RigidBodyAndCollisionLineSphere<CompressedScenePos>& d,
        const FixedArray<CompressedScenePos, 3>& ref)
{
    return d + (-ref);
}

inline AabbAndPayload<HalfCompressedScenePos, 3, RigidBodyAndCollisionLineSphere<HalfCompressedScenePos>>
    compress(
        const AabbAndPayload<CompressedScenePos, 3, RigidBodyAndCollisionLineSphere<CompressedScenePos>>& a,
        const FixedArray<CompressedScenePos, 3>& d)
{
    return { (a.primitive() - d).casted<HalfCompressedScenePos>(), (a.payload() - d).casted<HalfCompressedScenePos>() };
}

inline AabbAndPayload<CompressedScenePos, 3, RigidBodyAndCollisionLineSphere<CompressedScenePos>>
    decompress(
        const AabbAndPayload<HalfCompressedScenePos, 3, RigidBodyAndCollisionLineSphere<HalfCompressedScenePos>>& a,
        const FixedArray<CompressedScenePos, 3>& d)
{
    return {
        a.primitive().casted<CompressedScenePos>() + d,
        a.payload().casted<CompressedScenePos>() + d};
}

}
