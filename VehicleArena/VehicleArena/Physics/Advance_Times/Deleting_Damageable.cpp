// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Deleting_Damageable.hpp"
#include <VehicleArena/Audio/Audio_Entity_State.hpp>
#include <VehicleArena/Components/Rigid_Body_Vehicle.hpp>
#include <VehicleArena/Macro_Executor/Translator.hpp>
#include <VehicleArena/Memory/Object_Pool.hpp>
#include <VehicleArena/Physics/Containers/Advance_Times.hpp>
#include <VehicleArena/Physics/Interfaces/Damage_Source.hpp>
#include <VehicleArena/Physics/Rigid_Body/Rigid_Body_Vehicle.hpp>
#include <VehicleArena/Physics/Smoke_Generation/Smoke_Particle_Generator.hpp>
#include <VehicleArena/Scene_Graph/Containers/Scene.hpp>
#include <VehicleArena/Scene_Graph/Elements/Scene_Node.hpp>

using namespace VA;

DeletingDamageable::DeletingDamageable(
    Scene& scene,
    AdvanceTimes& advance_times,
    VariableAndHash<std::string> root_node_name,
    float health,
    bool delete_node_when_health_leq_zero,
    std::shared_ptr<Translator> translator,
    GenerateExplosions generate_explosions)
    : scene_{ scene }
    , advance_times_{ advance_times }
    , root_node_name_{ std::move(root_node_name) }
    , health_{ health }
    , delete_node_when_health_leq_zero_{ delete_node_when_health_leq_zero }
    , rb_{ &get_rigid_body_vehicle(scene.get_node(root_node_name_, DP_LOC)) }
    , translator_{ std::move(translator) }
    , generate_explosions_{ std::move(generate_explosions) }
    , node_on_clear_{ scene_.get_node(root_node_name_, DP_LOC)->on_clear, CURRENT_SOURCE_LOCATION }
    , rb_on_destroy_{ rb_->on_destroy, CURRENT_SOURCE_LOCATION }
    , final_damage_sources_{ DamageSource::NONE }
    , explosion_generated_{ false }
{
    if (rb_->damageable_ != nullptr) {
        throw std::runtime_error("Rigid body already has a damageable");
    }
    rb_->damageable_ = this;
    dgs_.add([this]() { if (rb_ != nullptr) { rb_->damageable_ = nullptr; } });
    advance_times_.add_advance_time({ *this, CURRENT_SOURCE_LOCATION }, CURRENT_SOURCE_LOCATION);
    node_on_clear_.add([this]() { global_object_pool.remove(this); }, CURRENT_SOURCE_LOCATION);
    rb_on_destroy_.add([this]() { rb_ = nullptr; global_object_pool.remove(this); }, CURRENT_SOURCE_LOCATION);
}

DeletingDamageable::~DeletingDamageable() {
    on_destroy.clear();
}

void DeletingDamageable::advance_time(float dt, const StaticWorld& world) {
    if (!explosion_generated_ && (health() <= 0)) {
        for (const auto& g : generate_explosions_) {
            if (!any(g.damage_sources & final_damage_sources_)) {
                continue;
            }
            if (!g.generate) {
                continue;
            }
            auto pos = rb_->rbp_.abs_position();
            g.generate({pos, rb_->velocity_at_position(pos)}, world);
        }
        if (delete_node_when_health_leq_zero_) {
            scene_.delete_root_node(root_node_name_);
            // The object is deleted from here on, so return.
            return;
        }
        explosion_generated_ = true;
    }
}

void DeletingDamageable::write_status(
    std::ostream& ostr,
    StatusComponents log_components,
    const StaticWorld& world) const
{
    static const VariableAndHash<std::string> HP{ "HP" };
    if (log_components & StatusComponents::HEALTH) {
        ostr << translator_->translate(HP) << ": " << health() << std::endl;
    }
}

float DeletingDamageable::get_value(StatusComponents log_components) const {
    if (log_components == StatusComponents::HEALTH) {
        return health();
    }
    throw std::runtime_error("Unknown status component: " + std::to_string((unsigned int)log_components));
}

StatusWriter& DeletingDamageable::child_status_writer(const std::vector<VariableAndHash<std::string>>& name) {
    throw std::runtime_error("DeletingDamageable has no children");
}

float DeletingDamageable::health() const {
    std::shared_lock lock{health_mutex_};
    return health_;
}

void DeletingDamageable::damage(float amount, DamageSource source) {
    std::scoped_lock lock{health_mutex_};
    health_ -= amount;
    if (health_ <= 0) {
        final_damage_sources_ |= source;
    }
}
