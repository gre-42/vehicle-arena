// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Array/Fixed_Array.hpp>
#include <VehicleArena/Memory/Dangling_Base_Class.hpp>
#include <VehicleArena/Memory/Dangling_Base_Class.hpp>
#include <VehicleArena/Memory/Dangling_Unique_Ptr.hpp>
#include <VehicleArena/Memory/Destruction_Observer.hpp>
#include <VehicleArena/Physics/Interfaces/IAdvance_Time.hpp>
#include <VehicleArena/Scene_Graph/Interfaces/Scene_Node/IRelative_Movable.hpp>
#include <functional>

namespace VA {

struct PhysicsEngineConfig;
class RigidBodyVehicle;
class AimAt;
class SceneNode;

class PitchLookAtNode: public DestructionObserver<SceneNode&>, public IRelativeMovable, public IAdvanceTime, public virtual DanglingBaseClass {
public:
    PitchLookAtNode(
        AimAt& aim_at,
        float pitch_min,
        float pitch_max,
        float dpitch_max,
        std::function<float()> increment_pitch_error);
    virtual ~PitchLookAtNode() override;
    virtual void set_initial_relative_model_matrix(const TransformationMatrix<float, ScenePos, 3>& relative_model_matrix) override;
    virtual void set_updated_relative_model_matrix(const TransformationMatrix<float, ScenePos, 3>& relative_model_matrix) override;
    virtual void set_absolute_model_matrix(const TransformationMatrix<float, ScenePos, 3>& absolute_model_matrix) override;
    virtual TransformationMatrix<float, ScenePos, 3> get_new_relative_model_matrix() const override;
    virtual void notify_destroyed(SceneNode& destroyed_object) override;
    virtual void advance_time(float dt, const StaticWorld& world) override;
    void increment_pitch(float dpitch, float relaxation);
    void goto_pitch(float pitch);
    void set_pitch(float pitch);
    float get_pitch() const;

    void set_head_node(DanglingBaseClassRef<SceneNode> head_node);

    void set_dpitch_head(float value);
    float get_dpitch_head() const;

private:
    AimAt& aim_at_node_;
    float dpitch_;
    float pitch_;
    float pitch_min_;
    float pitch_max_;
    float dpitch_max_;
    FixedArray<ScenePos, 3> relative_position_;
    float dpitch_head_;
    DanglingBaseClassPtr<SceneNode> head_node_;
    std::function<float()> increment_pitch_error_;
};

}
