// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Crash.hpp"
#include <VehicleArena/Geometry/Coordinates/Homogeneous.hpp>
#include <VehicleArena/Math/Fixed_Math.hpp>
#include <VehicleArena/Physics/Interfaces/Damage_Source.hpp>
#include <VehicleArena/Physics/Interfaces/IDamageable.hpp>
#include <VehicleArena/Physics/Interfaces/IPlayer.hpp>
#include <VehicleArena/Physics/Rigid_Body/Rigid_Body_Vehicle.hpp>
#include <VehicleArena/Physics/Units.hpp>
#include <VehicleArena/Scene_Graph/Base_Log.hpp>
#include <VehicleArena/Scene_Graph/Log_Entry_Severity.hpp>
#include <stdexcept>

using namespace VA;

Crash::Crash(
    RigidBodyVehicle& rigid_body,
    float damage)
    : rigid_body_{ rigid_body }
    , damage_{ damage }
{}

static float calculate_damage(
    const RigidBodyVehicle& rigid_body,
    float damage_raw,
    const FixedArray<float, 3>& normal,
    float lambda_final)
{
    auto fac = dot0d(dot1d(rigid_body.rbp_.rotation_, rigid_body.damage_absorption_direction_), normal);
    fac = std::sqrt(1 - std::min(1.f, squared(fac)));
    auto dv = -lambda_final / rigid_body.rbp_.mass_;
    return damage_raw * squared(std::max(0.f, dv / kph)) * fac;
}

void Crash::notify_impact(
    RigidBodyVehicle& rigid_body,
    PhysicsMaterial physics_material,
    CollisionRole collision_role,
    const FixedArray<float, 3>& normal,
    float lambda_final,
    BaseLog* base_log)
{
    if (collision_role == CollisionRole::PRIMARY) {
        float damage0 = NAN;
        for (auto& v : rigid_body.collision_observers_) {
            auto d = dynamic_cast<Crash*>(v.get());
            if (d != nullptr) {
                if (!std::isnan(damage0)) {
                    throw std::runtime_error("List contains multiple crashes");
                }
                damage0 = calculate_damage(rigid_body_, d->damage_, normal, lambda_final);
            }
        }
        if (!std::isnan(damage0)) {
            float damage1 = calculate_damage(rigid_body, damage_, normal, lambda_final);
            float min_damage = std::min(damage0, damage1);
            damage0 -= min_damage;
            damage1 -= min_damage;
            if (base_log != nullptr) {
                std::stringstream sstr;
                auto driver0 = rigid_body_.drivers_.try_get("driver");
                auto driver1 = rigid_body.drivers_.try_get("driver");
                if ((driver0 != nullptr) && (driver1 != nullptr)) {
                    if (damage0 != 0) {
                        sstr << driver1->title() << " -> " << driver0->title() << ": " << std::round(damage0) << " HP";
                    }
                    if (damage1 != 0) {
                        sstr << driver0->title() << " -> " << driver1->title() << ": " << std::round(damage1) << " HP";
                    }
                }
                if (damage0 < 1 && damage1 < 1) {
                    base_log->log(sstr.str(), LogEntrySeverity::INFO);
                } else {
                    base_log->log(sstr.str(), LogEntrySeverity::CRITICAL);
                }
            }
            if ((rigid_body_.damageable_ != nullptr) && (rigid_body_.damageable_->health() > 0)) {
                rigid_body_.damageable_->damage(damage0, DamageSource::CRASH);
                if (rigid_body_.damageable_->health() <= 0) {
                    auto driver = rigid_body.drivers_.try_get("driver");
                    if (driver != nullptr) {
                        driver->notify_kill(rigid_body_);
                    }
                }
            }
            if ((rigid_body.damageable_ != nullptr) && (rigid_body.damageable_->health() > 0)) {
                rigid_body.damageable_->damage(damage1, DamageSource::CRASH);
                if (rigid_body.damageable_->health() <= 0) {
                    auto driver = rigid_body_.drivers_.try_get("driver");
                    if (driver != nullptr) {
                        driver->notify_kill(rigid_body_);
                    }
                }
            }
        }
    }
}
