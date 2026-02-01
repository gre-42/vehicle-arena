// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Physics/Containers/Ridge_Map.hpp>
#include <VehicleArena/Scene_Config/Scene_Precision.hpp>
#include <chrono>
#include <list>
#include <map>
#include <memory>
#include <unordered_map>

namespace VA {

struct PhysicsEngineConfig;
class RigidBodies;
class ContactSmokeGenerator;
class ITrailRenderer;
struct Beacon;
class IContactInfo;
template <typename TData, size_t... tshape>
class FixedArray;
template <class TData, size_t... tshape>
class OrderableFixedArray;
struct IntersectionSceneAndContact;
class RigidBodyVehicle;
struct GrindInfo;
class BaseLog;
template <class TPosition>
struct CollisionRidgeSphere;
struct StaticWorld;
class SurfaceContactDb;
struct PhysicsPhase;

struct CollisionHistory {
    const PhysicsEngineConfig& cfg;
    const PhysicsPhase& phase;
    const StaticWorld& world;
    const SurfaceContactDb& surface_contact_db;
    ContactSmokeGenerator& csg;
    ITrailRenderer& tr;
    std::list<Beacon>* beacons;
    std::list<std::unique_ptr<IContactInfo>>& contact_infos;
    std::unordered_map<OrderableFixedArray<CompressedScenePos, 2, 3>, IntersectionSceneAndContact>& raycast_intersections;
    std::unordered_map<RigidBodyVehicle*, std::list<IntersectionSceneAndContact>>& concave_t0_intersections;
    std::unordered_map<RigidBodyVehicle*, GrindInfo>& grind_infos;
    BaseLog* base_log;
};

}
