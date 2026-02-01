// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Wing_Angle.hpp"
#include <VehicleArena/Math/Fixed_Rodrigues.hpp>
#include <VehicleArena/Math/Transformation/Transformation_Matrix.hpp>

using namespace VA;

WingAngle::WingAngle(DanglingBaseClassPtr<SceneNode> node, float& angle, const FixedArray<float, 3>& rotation_axis)
    : node_{ node }
    , angle_{ angle }
    , rotation_axis_{ rotation_axis }
    , position_{ fixed_nans<ScenePos, 3>() }
{
    if (node != nullptr) {
        node->set_relative_movable({ *this, CURRENT_SOURCE_LOCATION });
    }
}

WingAngle::~WingAngle() {
    if (node_ != nullptr) {
        node_->clearing_observers.remove(
            { *this, CURRENT_SOURCE_LOCATION },
            ObserverDoesNotExistBehavior::IGNORE);
        if (node_->has_relative_movable()) {
            if (&node_->get_relative_movable() != this) {
                verbose_abort("Unexpected relative movable");
            }
            node_->clear_relative_movable();
        }
    }
}

void WingAngle::notify_destroyed(SceneNode& destroyed_object) {
    if (destroyed_object.has_relative_movable()) {
        if (&destroyed_object.get_relative_movable() != this) {
            verbose_abort("Unexpected relative movable");
        }
        destroyed_object.clear_relative_movable();
    }
    node_ = nullptr;
}

void WingAngle::set_initial_relative_model_matrix(const TransformationMatrix<float, ScenePos, 3>& relative_model_matrix) {
    position_ = relative_model_matrix.t;
}

void WingAngle::set_updated_relative_model_matrix(const TransformationMatrix<float, ScenePos, 3>& relative_model_matrix) {
    // Do nothing
}

void WingAngle::set_absolute_model_matrix(const TransformationMatrix<float, ScenePos, 3>& absolute_model_matrix) {
    // Do nothing
}

TransformationMatrix<float, ScenePos, 3> WingAngle::get_new_relative_model_matrix() const {
    return TransformationMatrix<float, ScenePos, 3>{
        rodrigues2(rotation_axis_, angle_),
        position_};
}
