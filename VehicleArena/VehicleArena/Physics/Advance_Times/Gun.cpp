// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Gun.hpp"
#include <VehicleArena/Audio/Audio_Entity_State.hpp>
#include <VehicleArena/Components/Rigid_Body_Vehicle.hpp>
#include <VehicleArena/Memory/Object_Pool.hpp>
#include <VehicleArena/Physics/Advance_Times/Bullet.hpp>
#include <VehicleArena/Physics/Bullets/Bullet_Generator.hpp>
#include <VehicleArena/Physics/Bullets/Bullet_Properties.hpp>
#include <VehicleArena/Physics/Collision/Collidable_Mode.hpp>
#include <VehicleArena/Physics/Containers/Advance_Times.hpp>
#include <VehicleArena/Physics/Interfaces/IPlayer.hpp>
#include <VehicleArena/Physics/Rigid_Body/Rigid_Body_Vehicle.hpp>
#include <VehicleArena/Scene_Graph/Instances/Static_World.hpp>

using namespace VA;

Gun::Gun(
    AdvanceTimes& advance_times,
    float cool_down,
    RigidBodyVehicle& parent_rb,
    const DanglingBaseClassRef<SceneNode>& node,
    const DanglingBaseClassPtr<SceneNode>& punch_angle_node,
    const BulletProperties& bullet_properties,
    std::function<void(
        const std::optional<VariableAndHash<std::string>>& player,
        const std::string& bullet_suffix,
        const std::optional<VariableAndHash<std::string>>& target,
        const FixedArray<float, 3>& velocity,
        const FixedArray<float, 3>& angular_velocity)> generate_smart_bullet,
    std::function<void(const AudioSourceState<ScenePos>&)> generate_shot_audio,
    const BulletGenerator& bullet_generator,
    std::string ammo_type,
    std::function<FixedArray<float, 3>(bool shooting)> punch_angle_rng,
    std::function<void(const StaticWorld&)> generate_muzzle_flash)
    : advance_times_{ advance_times }
    , parent_rb_{ parent_rb }
    , node_{ node.ptr() }
    , punch_angle_node_{ punch_angle_node }
    , ypln_node_{ nullptr }
    , bullet_properties_{ bullet_properties }
    , generate_smart_bullet_{ std::move(generate_smart_bullet) }
    , generate_shot_audio_{ std::move(generate_shot_audio) }
    , bullet_generator_{ bullet_generator }
    , ammo_type_{ std::move(ammo_type) }
    , triggered_{ false }
    , player_{ nullptr }
    , team_{ nullptr }
    , cool_down_{ cool_down }
    , time_since_last_shot_{ 0 }
    , absolute_model_matrix_{ fixed_nans<ScenePos, 4, 4 >() }
    , punch_angle_{ 0.f, 0.f, 0.f }
    , punch_angle_rng_{ std::move(punch_angle_rng) }
    , generate_muzzle_flash_{ std::move(generate_muzzle_flash) }
    , node_on_clear_{ node->on_clear, CURRENT_SOURCE_LOCATION }
{
    if (punch_angle_node != nullptr) {
        punch_angle_node_on_clear_.emplace(punch_angle_node->on_clear, CURRENT_SOURCE_LOCATION);
        punch_angle_node_on_clear_->add(
            [this]() { punch_angle_node_ = nullptr; },
            CURRENT_SOURCE_LOCATION);
    }
    node->set_absolute_observer(*this);
    dgs_.add([node]() { if (node->has_absolute_observer()) { node->clear_absolute_observer(); }});
    advance_times_.add_advance_time({ *this, CURRENT_SOURCE_LOCATION }, CURRENT_SOURCE_LOCATION);
    node_on_clear_.add(
        [this]() { node_ = nullptr; global_object_pool.remove(this); },
        CURRENT_SOURCE_LOCATION);
}

Gun::~Gun() {
    on_destroy.clear();
}

void Gun::advance_time(float dt, const StaticWorld& world) {
    time_since_last_shot_ += dt;
    time_since_last_shot_ = std::min(time_since_last_shot_, cool_down_);
    punch_angle_ = punch_angle_rng_(maybe_generate_bullet(world));
    if (punch_angle_node_ != nullptr) {
        punch_angle_node_->set_rotation(punch_angle_, SUCCESSOR_POSE);
    }
    triggered_ = false;
}

size_t Gun::nbullets_available() const {
    return parent_rb_.inventory_.navailable(ammo_type_);
}

bool Gun::maybe_generate_bullet(const StaticWorld& world) {
    if (is_none_gun()) {
        return false;
    }
    if (!triggered_) {
        return false;
    }
    if (time_since_last_shot_ != cool_down_) {
        return false;
    }
    if (nbullets_available() == 0) {
        return false;
    }
    if (generate_smart_bullet_ &&
        (player_ != nullptr) &&
        !player_->target_id().has_value())
    {
        return false;
    }
    parent_rb_.inventory_.take(ammo_type_, 1);
    time_since_last_shot_ = 0;
    generate_bullet();
    if (generate_muzzle_flash_) {
        generate_muzzle_flash(world);
    }
    if (generate_shot_audio_) {
        generate_shot_audio();
    }
    if (player_ != nullptr) {
        player_->notify_bullet_generated(world.time);
    }
    return true;
}

void Gun::generate_bullet() {
    auto bullet_velocity =
        - bullet_properties_.velocity * z3_from_3x3(absolute_model_matrix_.R)
        + parent_rb_.velocity_at_position(absolute_model_matrix_.t);
    bullet_generator_.generate_bullet(
        bullet_properties_,
        generate_smart_bullet_,
        &parent_rb_,
        absolute_model_matrix_,
        bullet_velocity,
        fixed_zeros<float, 3>(), // angular_velocities
        player_,
        team_);
}

void Gun::generate_muzzle_flash(const StaticWorld& world) {
    if (!generate_muzzle_flash_) {
        throw std::runtime_error("Muzzle flash hider not set");
    }
    generate_muzzle_flash_(world);
}

void Gun::generate_shot_audio() {
    if (!generate_shot_audio_) {
        throw std::runtime_error("Shot audio not set");
    }
    generate_shot_audio_({
        absolute_model_matrix_.t,
        parent_rb_.velocity_at_position(absolute_model_matrix_.t)});
}

void Gun::set_absolute_model_matrix(const TransformationMatrix<float, ScenePos, 3>& absolute_model_matrix)
{
    absolute_model_matrix_ = absolute_model_matrix;
}

void Gun::trigger(IPlayer* player, ITeam* team) {
    triggered_ = true;
    player_ = player;
    team_ = team;
}

const TransformationMatrix<float, ScenePos, 3>& Gun::absolute_model_matrix() const {
    return absolute_model_matrix_;
}

bool Gun::is_none_gun() const {
    return bullet_properties_.max_lifetime == 0;
}

const FixedArray<float, 3>& Gun::punch_angle() const {
    return punch_angle_;
}

float Gun::cool_down() const {
    return cool_down_;
}

float Gun::bullet_damage() const {
    return bullet_properties_.damage;
}

void Gun::set_ypln_node(const DanglingBaseClassRef<SceneNode>& node) {
    if (ypln_node_ != nullptr) {
        throw std::runtime_error("YPLN node already set");
    }
    ypln_node_ = node.ptr();
    ypln_node_on_clear_.emplace(ypln_node_->on_clear, CURRENT_SOURCE_LOCATION);
    punch_angle_node_on_clear_->add(
        [this]() { ypln_node_ = nullptr; },
        CURRENT_SOURCE_LOCATION);
}

DanglingBaseClassPtr<SceneNode> Gun::get_ypln_node() const {
    return ypln_node_;
}
