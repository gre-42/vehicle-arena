// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Array/Fixed_Array.hpp>
#include <VehicleArena/Geometry/Primitives/Collision_Line.hpp>
#include <VehicleArena/Geometry/Primitives/Collision_Polygon.hpp>
#include <VehicleArena/Geometry/Primitives/Collision_Ridge.hpp>
#include <VehicleArena/Geometry/Primitives/Intersectors/Intersection_Info.hpp>
#include <VehicleArena/Scene_Config/Scene_Precision.hpp>
#include <cstdint>
#include <memory>
#include <vector>

namespace VA {

class RigidBodyVehicle;
class IIntersectableMesh;
template <class TDir, class TPos, size_t tndim>
class PlaneNd;
enum class CollisionType;
enum class PhysicsMaterial: uint32_t;
struct IntersectionSceneAndContact;
struct CollisionHistory;
template <class TPosition, size_t tnvertices>
struct CollisionPolygonSphere;
template <class TPosition>
struct CollisionRidgeSphere;
template <class TPosition>
struct CollisionLineSphere;
struct SurfaceContactInfo;
class IIntersectable;

struct IntersectionScene {
    RigidBodyVehicle& o0;
    RigidBodyVehicle& o1;
    const IIntersectableMesh* mesh0;
    const IIntersectableMesh* mesh1;
    const std::optional<CollisionLineSphere<CompressedScenePos>> l1;
    const std::optional<CollisionRidgeSphere<CompressedScenePos>> r1;
    const IIntersectable* i1 = nullptr;
    std::optional<CollisionPolygonSphere<CompressedScenePos, 4>> q0;
    std::optional<CollisionPolygonSphere<CompressedScenePos, 3>> t0;
    const IIntersectable* i0 = nullptr;
    size_t tire_id1;
    PhysicsMaterial mesh0_material;
    PhysicsMaterial mesh1_material;
    bool l1_is_normal;
    const SurfaceContactInfo* surface_contact_info;
    CollisionType default_collision_type;
    const CollisionHistory& history;
};

struct IntersectionSceneAndContact {
    IntersectionScene scene;
    IntersectionInfo iinfo;
};

}
