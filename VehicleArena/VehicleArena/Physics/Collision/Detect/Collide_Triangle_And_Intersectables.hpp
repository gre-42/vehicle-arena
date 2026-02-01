// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Scene_Config/Scene_Precision.hpp>
#include <memory>
#include <variant>

namespace VA {

class RigidBodyVehicle;
template <class T>
struct TypedMesh;
class IIntersectableMesh;
template <class TPosition, size_t tnvertices>
struct CollisionPolygonSphere;
struct CollisionHistory;

void collide_triangle_and_intersectables(
    RigidBodyVehicle& o0,
    RigidBodyVehicle& o1,
    const TypedMesh<std::shared_ptr<IIntersectableMesh>>& msh1,
    const CollisionPolygonSphere<CompressedScenePos, 4>* q0,
    const CollisionPolygonSphere<CompressedScenePos, 3>* t0,
    const CollisionHistory& history);

void collide_triangle_and_intersectables(
    RigidBodyVehicle& o0,
    RigidBodyVehicle& o1,
    const TypedMesh<std::shared_ptr<IIntersectableMesh>>& msh1,
    const std::variant<CollisionPolygonSphere<CompressedScenePos, 3>, CollisionPolygonSphere<CompressedScenePos, 4>>& cps0,
    const CollisionHistory& history);

}
