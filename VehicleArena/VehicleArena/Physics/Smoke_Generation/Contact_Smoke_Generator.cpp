// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Contact_Smoke_Generator.hpp"
#include <VehicleArena/Audio/Audio_Entity_State.hpp>
#include <VehicleArena/Audio/Audio_Periodicity.hpp>
#include <VehicleArena/Geometry/Material/Particle_Type.hpp>
#include <VehicleArena/Math/Lerp.hpp>
#include <VehicleArena/Physics/Collision/Record/Collision_History.hpp>
#include <VehicleArena/Physics/Collision/Record/Intersection_Scene.hpp>
#include <VehicleArena/Physics/Physics_Engine/Physics_Phase.hpp>
#include <VehicleArena/Physics/Rigid_Body/Rigid_Body_Vehicle.hpp>
#include <VehicleArena/Physics/Smoke_Generation/Smoke_Particle_Generator.hpp>
#include <VehicleArena/Physics/Smoke_Generation/Surface_Contact_Info.hpp>
#include <VehicleArena/Scene_Config/Physics_Engine_Config.hpp>
#include <VehicleArena/Scene_Graph/Instances/Static_World.hpp>
#include <stdexcept>

using namespace VA;

ContactSmokeGenerator::ContactSmokeGenerator(
    OneShotAudio& one_shot_audio,
    SmokeParticleGenerator& air_smoke_particle_generator,
    SmokeParticleGenerator& skidmark_smoke_particle_generator,
    SmokeParticleGenerator& sea_wave_smoke_particle_generator)
    : one_shot_audio_{ one_shot_audio }
    , air_smoke_particle_generator_{ air_smoke_particle_generator }
    , skidmark_smoke_particle_generator_{ skidmark_smoke_particle_generator }
    , sea_wave_smoke_particle_generator_{ sea_wave_smoke_particle_generator }
{}

ContactSmokeGenerator::~ContactSmokeGenerator() {
    if (!tire_smoke_trail_generators_.empty()) {
        verbose_abort("Tire smoke generators remain during shutdown");
    }
}

void ContactSmokeGenerator::notify_destroyed(const RigidBodyVehicle& destroyed_object) {
    if (tire_smoke_trail_generators_.erase(const_cast<RigidBodyVehicle*>(&destroyed_object)) != 1) {
        verbose_abort("Could not find surface contact info to be deleted");
    }
}

void ContactSmokeGenerator::notify_contact(
    const FixedArray<ScenePos, 3>& intersection_point,
    const FixedArray<float, 3>& rotation,
    const FixedArray<SceneDir, 3>& surface_normal,
    const IntersectionScene& c)
{
    if (c.history.phase.burn_in) {
        return;
    }
    if (c.surface_contact_info == nullptr) {
        return;
    }
    if (c.surface_contact_info->emission.empty()) {
        return;
    }
    auto v0 = c.o0.rbp_.velocity_at_position(intersection_point);
    auto v1_a = c.o1.rbp_.velocity_at_position(intersection_point);
    auto v1_s =
        c.o1.get_velocity_at_tire_contact(surface_normal.casted<float>(), c.tire_id1) +
        c.o1.get_abs_tire_z(c.tire_id1) *
        (
            c.o1.get_tire_angular_velocity(c.tire_id1) *
            c.o1.get_tire_radius(c.tire_id1));
    auto dvel_a = std::sqrt(sum(squared(v0 - v1_a)));
    auto dvel_s = std::sqrt(sum(squared(v0 - v1_s)));
    for (const auto& smoke_info : c.surface_contact_info->emission) {
        const auto& af = smoke_info.vehicle_velocity.smoke_particle_frequency;
        const auto& sf = smoke_info.tire_velocity.smoke_particle_frequency;
        const auto& sl = smoke_info.tire_velocity.smoke_particle_layer;
        auto f =
            (af.empty() ? 0.f : af(dvel_a)) +
            (sf.empty() ? 0.f : sf(dvel_s));
        if (f < 1e-12f) {
            continue;
        }
        auto texture_layer = sl.empty() ? 0.f : sl(dvel_s);
        if (!sl.empty() && (texture_layer < 1e-12f)) {
            continue;
        }
        auto& tstg = [&]() -> ContactSmokeAndAudio& {
            auto it = tire_smoke_trail_generators_.find(&c.o1);
            if (it == tire_smoke_trail_generators_.end()) {
                auto res = tire_smoke_trail_generators_.try_emplace(&c.o1);
                if (!res.second) {
                    verbose_abort("ContactSmokeGenerator tire_smoke_trail_generators_.try_emplace error");
                }
                c.o1.destruction_observers.add({ *this, CURRENT_SOURCE_LOCATION }, ObserverAlreadyExistsBehavior::IGNORE);
                return res.first->second;
            }
            return it->second;
        }();
        auto ce_generate = [&](ContactEmissions& ce){
            if (auto n = ce.maybe_generate(1.f / f); n != 0) {
                if (smoke_info.audio != nullptr) {
                    smoke_info.audio->play(
                        one_shot_audio_,
                        AudioPeriodicity::APERIODIC,
                        {intersection_point, c.o1.rbp_.velocity_at_position(intersection_point)});
                }
                if (smoke_info.visual.has_value()) {
                    assert_true(ce.particle_trail_generator.has_value());
                    const auto& av = smoke_info.vehicle_velocity.smoke_particle_velocity;
                    const auto& sv = smoke_info.tire_velocity.smoke_particle_velocity;
                    auto pvel =
                        (av.empty() ? 0.f : av(dvel_a)) +
                        (sv.empty() ? 0.f : sv(dvel_s));
                    auto dirx = c.o1.rbp_.rotation_.column(0);
                    if (dot0d(dirx, (intersection_point - c.o1.rbp_.abs_position()).casted<float>()) < 0.f) {
                        dirx = -dirx;
                    }
                    dirx -= surface_normal.casted<float>() * dot0d(surface_normal.casted<float>(), dirx);
                    auto ptg_generate = [&](const FixedArray<ScenePos, 3>& p){
                        ce.particle_trail_generator->generate(
                            p,
                            rotation,
                            dirx * pvel,
                            texture_layer,
                            smoke_info.visual->particle,
                            smoke_info.visual->smoke_particle_instance_prefix,
                            ParticleContainer::INSTANCE,
                            c.history.world);
                    };
                    if (!ce.old_position.has_value() || (n > c.history.cfg.max_interpolated_particles)) {
                        ptg_generate(intersection_point);
                    } else {
                        for (uint32_t i = 1; i <= n; ++i) {
                            ptg_generate(lerp(*ce.old_position, intersection_point, integral_to_float<ScenePos>(i) / n));
                        }
                    }
                    ce.old_position.emplace(intersection_point);
                }
            }
        };
        auto create_particle_trail_generator = [&](ContactEmissions& ce){
            if (smoke_info.visual.has_value()) {
                auto& pgen = [&]() -> SmokeParticleGenerator& {
                    switch (smoke_info.visual->particle.type) {
                        case ParticleType::NONE: throw std::runtime_error("Particle type \"none\" does not require a contact smoke generator");
                        case ParticleType::SMOKE: return air_smoke_particle_generator_;
                        case ParticleType::SKIDMARK: return skidmark_smoke_particle_generator_;
                        case ParticleType::WATER_WAVE: throw std::runtime_error("Water waves do not require a contact smoke generator");
                        case ParticleType::SEA_SPRAY: return sea_wave_smoke_particle_generator_;
                    };
                    throw std::runtime_error("Unknown particle type");
                }();
                ce.particle_trail_generator.emplace(pgen);
            }
        };
        auto generate = [&](const auto& key, auto& map){
            if (auto tstgit = map.find(key); tstgit == map.end()) {
                auto ceit = map.try_emplace(key);
                if (!ceit.second) {
                    verbose_abort("Could not insert smoke trail generator");
                }
                ContactEmissions& ce = ceit.first->second;
                create_particle_trail_generator(ce);
                ce_generate(ce);
            } else {
                ce_generate(tstgit->second);
            }
        };
        switch (smoke_info.affinity) {
        case SurfaceSmokeAffinity::PAIR: {
                std::pair<size_t, const SurfaceSmokeInfo*> key{ c.tire_id1, &smoke_info };
                generate(key, tstg.smoke);
                continue;
            }
        case SurfaceSmokeAffinity::TIRE: {
                size_t key = c.tire_id1;
                generate(key, tstg.audio);
                continue;
            }
        };
        throw std::runtime_error("Unknown surface from affinity");
    }
}

void ContactSmokeGenerator::advance_time(
    RigidBodyVehicle& vehicle,
    const PhysicsEngineConfig& cfg,
    const PhysicsPhase& phase)
{
    if (!phase.group.rigid_bodies.contains(&vehicle.rbp_)) {
        return;
    }
    auto it = tire_smoke_trail_generators_.find(&vehicle);
    if (it == tire_smoke_trail_generators_.end()) {
        return;
    }
    auto dt = cfg.dt_substeps(phase);
    for (auto& [_1, g] : it->second.smoke) {
        g.maybe_generate.advance_time(dt);
    }
    for (auto& [_1, g] : it->second.audio) {
        g.maybe_generate.advance_time(dt);
    }
}
