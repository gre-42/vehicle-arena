// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Geometry/Primitives/Bvh.hpp>
#include <VehicleArena/Geometry/Primitives/Collision_Polygon.hpp>
#include <VehicleArena/Scene_Config/Scene_Precision.hpp>

namespace VA {

class RigidBodyVehicle;

template <class TPosition>
struct RigidBodyAndCollisionTriangleSphere {
    RigidBodyVehicle& rb;
    std::variant<CollisionPolygonSphere<TPosition, 3>, CollisionPolygonSphere<TPosition, 4>> ctp;
    template <class TResult>
    RigidBodyAndCollisionTriangleSphere<TResult> casted() const {
        std::optional<RigidBodyAndCollisionTriangleSphere<TResult>> result;
        std::visit([&](auto& ctp){
            result.emplace(rb, ctp.template casted<TResult>());
        }, ctp);
        return *result;
    }
    bool operator == (const RigidBodyAndCollisionTriangleSphere& other) const {
        return (&rb == &other.rb) &&
               (ctp == other.ctp);
    }
};

inline RigidBodyAndCollisionTriangleSphere<CompressedScenePos>
    operator + (
        const RigidBodyAndCollisionTriangleSphere<CompressedScenePos>& d,
        const FixedArray<CompressedScenePos, 3>& ref)
{
    std::optional<RigidBodyAndCollisionTriangleSphere<CompressedScenePos>> result;
    std::visit([&](auto& ctp){
        result.emplace(d.rb, ctp + ref);
    }, d.ctp);
    return *result;
}

inline RigidBodyAndCollisionTriangleSphere<CompressedScenePos>
    operator - (
        const RigidBodyAndCollisionTriangleSphere<CompressedScenePos>& d,
        const FixedArray<CompressedScenePos, 3>& ref)
{
    return d + (-ref);
}

inline AabbAndPayload<HalfCompressedScenePos, 3, RigidBodyAndCollisionTriangleSphere<HalfCompressedScenePos>>
    compress(
        const AabbAndPayload<CompressedScenePos, 3, RigidBodyAndCollisionTriangleSphere<CompressedScenePos>>& a,
        const FixedArray<CompressedScenePos, 3>& d)
{
    return { (a.primitive() - d).casted<HalfCompressedScenePos>(), (a.payload() - d).casted<HalfCompressedScenePos>() };
}

inline AabbAndPayload<CompressedScenePos, 3, RigidBodyAndCollisionTriangleSphere<CompressedScenePos>>
    decompress(
        const AabbAndPayload<HalfCompressedScenePos, 3, RigidBodyAndCollisionTriangleSphere<HalfCompressedScenePos>>& a,
        const FixedArray<CompressedScenePos, 3>& d)
{
    return {
        a.primitive().casted<CompressedScenePos>() + d,
        a.payload().casted<CompressedScenePos>() + d};
}

}
