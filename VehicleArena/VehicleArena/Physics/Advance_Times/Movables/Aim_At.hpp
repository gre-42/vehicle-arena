// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Array/Fixed_Array.hpp>
#include <VehicleArena/Math/Transformation/Transformation_Matrix.hpp>
#include <VehicleArena/Memory/Dangling_Base_Class.hpp>
#include <VehicleArena/Memory/Dangling_Unique_Ptr.hpp>
#include <VehicleArena/Memory/Destruction_Functions.hpp>
#include <VehicleArena/Memory/Destruction_Guards.hpp>
#include <VehicleArena/Misc/Object.hpp>
#include <VehicleArena/Physics/Interfaces/IAdvance_Time.hpp>
#include <VehicleArena/Scene_Graph/Interfaces/Scene_Node/IAbsolute_Observer.hpp>
#include <functional>

namespace VA {

class PitchLookAtNode;
class AdvanceTimes;
struct PhysicsEngineConfig;
class SceneNode;
class RigidBodyVehicle;

class AimAt: public IAbsoluteObserver, public IAdvanceTime, public virtual DanglingBaseClass {
public:
    AimAt(
        AdvanceTimes& advance_times,
        DanglingBaseClassRef<SceneNode> follower_node,
        DanglingBaseClassRef<SceneNode> gun_node,
        float bullet_start_offset,
        float bullet_velocity,
        bool bullet_feels_gravity,
        float gravity,
        float locked_on_cosine,
        std::function<float()> velocity_estimation_error);
    ~AimAt();
    virtual void set_absolute_model_matrix(const TransformationMatrix<float, ScenePos, 3>& absolute_model_matrix) override;
    virtual void advance_time(float dt, const StaticWorld& world) override;

    bool has_followed() const;
    void set_followed(DanglingBaseClassPtr<SceneNode> followed_node);

    bool target_locked_on() const;
    void set_bullet_velocity(float value);
    void set_bullet_feels_gravity(bool value);

    const FixedArray<ScenePos, 3>& absolute_point_to_aim_at() const;
    const FixedArray<ScenePos, 3>& relative_point_to_aim_at() const;

private:
    FixedArray<ScenePos, 3> absolute_point_to_aim_at_;
    FixedArray<ScenePos, 3> relative_point_to_aim_at_;
    DanglingBaseClassPtr<SceneNode> followed_node_;
    DanglingBaseClassPtr<SceneNode> gun_node_;
    const RigidBodyVehicle& follower_;
    const RigidBodyVehicle* followed_;
    float bullet_start_offset_;
    float bullet_velocity_;
    bool bullet_feels_gravity_;
    float gravity_;
    float locked_on_cosine_;
    bool target_locked_on_;
    std::function<float()> velocity_estimation_error_;
    DestructionGuards dgs_;
    DestructionFunctionsRemovalTokens gun_node_on_destroy_;
    DestructionFunctionsRemovalTokens follower_node_on_destroy_;
    DestructionFunctionsRemovalTokens followed_node_on_destroy_;
};

}
