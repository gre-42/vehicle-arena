// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Avatar_As_Avatar_Controller.hpp"
#include <VehicleArena/Physics/Actuators/Engine_Power_Intent.hpp>
#include <VehicleArena/Physics/Advance_Times/Movables/Pitch_Look_At_Node.hpp>
#include <VehicleArena/Physics/Advance_Times/Movables/Yaw_Pitch_Look_At_Nodes.hpp>
#include <VehicleArena/Physics/Rigid_Body/Rigid_Body_Vehicle.hpp>
#include <VehicleArena/Scene_Graph/Animation/Animation_State_Updater.hpp>
#include <stdexcept>

using namespace VA;

AvatarAsAvatarController::AvatarAsAvatarController(
    RigidBodyVehicle& rb,
    YawPitchLookAtNodes& ypln)
    : rb_ { rb }
    , ypln_{ ypln }
{}

AvatarAsAvatarController::~AvatarAsAvatarController()
{}

static const auto legs_name = VariableAndHash<std::string>{ "legs" };

void AvatarAsAvatarController::apply() {
    if ((any(abs(legs_z_) > float(1e-12))) && (drive_relaxation_ > 0.f)) {
        rb_.tires_z_ = legs_z_ / std::sqrt(sum(squared(legs_z_)));
        rb_.set_surface_power(legs_name, EnginePowerIntent{
            .surface_power = surface_power_,
            .drive_relaxation = drive_relaxation_});
    } else {
        rb_.tires_z_ = { 0.f, 0.f, 1.f };
        rb_.set_surface_power(legs_name, EnginePowerIntent{.surface_power = NAN});
    }
    if (!std::isnan(target_yaw_)) {
        ypln_.goto_yaw(target_yaw_);
    }
    if (!std::isnan(target_pitch_)) {
        ypln_.pitch_look_at_node().goto_pitch(target_pitch_);
    }
    if (!std::isnan(dyaw_)) {
        ypln_.increment_yaw(dyaw_, dyaw_relaxation_);
    }
    if (!std::isnan(dpitch_)) {
        ypln_.pitch_look_at_node().increment_pitch(dpitch_, dpitch_relaxation_);
    }
    if (rb_.animation_state_updater_ != nullptr) {
        rb_.animation_state_updater_->notify_movement_intent();
    }
}
