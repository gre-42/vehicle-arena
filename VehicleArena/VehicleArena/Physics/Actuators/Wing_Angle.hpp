// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Array/Fixed_Array.hpp>
#include <VehicleArena/Memory/Dangling_Base_Class.hpp>
#include <VehicleArena/Memory/Dangling_Unique_Ptr.hpp>
#include <VehicleArena/Scene_Graph/Elements/Scene_Node.hpp>
#include <VehicleArena/Scene_Graph/Interfaces/Scene_Node/IRelative_Movable.hpp>

namespace VA {

class WingAngle: public DestructionObserver<SceneNode&>, public IRelativeMovable, public virtual DanglingBaseClass {
public:
    explicit WingAngle(DanglingBaseClassPtr<SceneNode> node, float& angle, const FixedArray<float, 3>& rotation_axis);
    virtual ~WingAngle() override;

    // DestructionObserver
    virtual void notify_destroyed(SceneNode& destroyed_object) override;

    // IRelativeMovable
    virtual void set_initial_relative_model_matrix(const TransformationMatrix<float, ScenePos, 3>& relative_model_matrix) override;
    virtual void set_updated_relative_model_matrix(const TransformationMatrix<float, ScenePos, 3>& relative_model_matrix) override;
    virtual void set_absolute_model_matrix(const TransformationMatrix<float, ScenePos, 3>& absolute_model_matrix) override;
    virtual TransformationMatrix<float, ScenePos, 3> get_new_relative_model_matrix() const override;
private:
    DanglingBaseClassPtr<SceneNode> node_;
    float& angle_;
    FixedArray<float, 3> rotation_axis_;
    FixedArray<ScenePos, 3> position_;
};

}
