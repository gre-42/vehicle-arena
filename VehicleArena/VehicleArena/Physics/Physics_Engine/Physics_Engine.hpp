// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Physics/Containers/Advance_Times.hpp>
#include <VehicleArena/Physics/Containers/Collision_Query.hpp>
#include <VehicleArena/Physics/Containers/Permanent_Contacts.hpp>
#include <VehicleArena/Physics/Containers/Rigid_Bodies.hpp>
#include <VehicleArena/Scene_Config/Physics_Engine_Config.hpp>
#include <chrono>
#include <list>
#include <set>

namespace VA {

class IExternalForceProvider;
class IControllable;
struct Beacon;
class BaseLog;
enum class CollisionDirection;
class SurfaceContactDb;
class ContactSmokeGenerator;
class ITrailRenderer;
struct StaticWorld;
struct PhysicsPhase;

class PhysicsEngine {
public:
    explicit PhysicsEngine(const PhysicsEngineConfig& cfg);
    ~PhysicsEngine();
    void add_external_force_provider(IExternalForceProvider& efp);
    void remove_external_force_provider(IExternalForceProvider& efp);
    void add_controllable(IControllable& co);
    void remove_controllable(IControllable& co);
    void compute_transformed_objects(const PhysicsPhase* phase);
    void collide(
        const StaticWorld& world,
        std::list<Beacon>* beacons,
        const PhysicsPhase& phase,
        BaseLog* base_log);
    void move_rigid_bodies(
        const StaticWorld& world,
        std::list<Beacon>* beacons,
        const PhysicsPhase& phase);
    void move_particles(const StaticWorld& world);
    void move_advance_times(const StaticWorld& world);
    void burn_in(
        const StaticWorld& world,
        float duration);
    void set_surface_contact_db(SurfaceContactDb& surface_contact_db);
    void set_contact_smoke_generator(ContactSmokeGenerator& contact_smoke_generator);
    void set_trail_renderer(ITrailRenderer& trail_renderer);
    inline const PhysicsEngineConfig& config() const { return cfg_; }

    RigidBodies rigid_bodies_;
    AdvanceTimes advance_times_;
    CollisionQuery collision_query_;
    PermanentContacts permanent_contacts_;
private:
    CollisionDirection collision_direction_;
    SurfaceContactDb* surface_contact_db_;
    ContactSmokeGenerator* contact_smoke_generator_;
    ITrailRenderer* trail_renderer_;
    std::list<IExternalForceProvider*> external_force_providers_;
    std::set<IControllable*> controllables_;
    PhysicsEngineConfig cfg_;
};

}
