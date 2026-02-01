// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Copy_Rotation.hpp"
#include <VehicleArena/Geometry/Coordinates/Homogeneous.hpp>
#include <VehicleArena/Memory/Object_Pool.hpp>
#include <VehicleArena/Physics/Containers/Advance_Times.hpp>
#include <VehicleArena/Scene_Graph/Elements/Scene_Node.hpp>

using namespace VA;

CopyRotation::CopyRotation(DanglingBaseClassRef<SceneNode> from)
    : from_{ from.ptr() }
    , transformation_matrix_{ fixed_nans<float, 3, 3>(), fixed_nans<ScenePos, 3>() }
{}

CopyRotation::~CopyRotation() {
    on_destroy.clear();
}

void CopyRotation::set_initial_relative_model_matrix(const TransformationMatrix<float, ScenePos, 3>& relative_model_matrix)
{
    transformation_matrix_ = relative_model_matrix;
}

void CopyRotation::set_updated_relative_model_matrix(const TransformationMatrix<float, ScenePos, 3>& relative_model_matrix)
{
    transformation_matrix_.t = relative_model_matrix.t;
}

void CopyRotation::set_absolute_model_matrix(const TransformationMatrix<float, ScenePos, 3>& absolute_model_matrix)
{
    if (from_ == nullptr) {
        return;
    }
    transformation_matrix_.R = from_->relative_model_matrix().R;
}

TransformationMatrix<float, ScenePos, 3> CopyRotation::get_new_relative_model_matrix() const
{
    return transformation_matrix_;
}

void CopyRotation::advance_time(float dt, const StaticWorld& world) {
    // Do nothing
}

void CopyRotation::notify_destroyed(SceneNode& destroyed_object) {
    if (&destroyed_object == from_.get()) {
        from_ = nullptr;
    } else {
        if (from_ != nullptr) {
            from_->clearing_observers.remove({ *this, CURRENT_SOURCE_LOCATION });
        }
        global_object_pool.remove(this);
    }
}
