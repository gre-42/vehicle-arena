// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Wheel.hpp"
#include <VehicleArena/Geometry/Coordinates/Homogeneous.hpp>
#include <VehicleArena/Math/Fixed_Rodrigues.hpp>
#include <VehicleArena/Memory/Dangling_Unique_Ptr.hpp>
#include <VehicleArena/Memory/Object_Pool.hpp>
#include <VehicleArena/Physics/Actuators/Tire.hpp>
#include <VehicleArena/Physics/Containers/Advance_Times.hpp>
#include <VehicleArena/Physics/Rigid_Body/Rigid_Body_Vehicle.hpp>
#include <VehicleArena/Scene_Graph/Elements/Scene_Node.hpp>

using namespace VA;

Wheel::Wheel(
    RigidBodyVehicle& rigid_body,
    AdvanceTimes& advance_times,
    size_t tire_id,
    float radius)
    : rigid_body_{ rigid_body }
    , advance_times_{ advance_times }
    , transformation_matrix_{
          fixed_nans<float, 3, 3>(),
          fixed_nans<ScenePos, 3>() }
          , tire_id_{ tire_id }
    , angle_x_{ 0 }
    , radius_{ radius }
    , y0_{ NAN }
{}

Wheel::~Wheel() {
    on_destroy.clear();
}

void Wheel::set_initial_relative_model_matrix(const TransformationMatrix<float, ScenePos, 3>& relative_model_matrix)
{
    transformation_matrix_ = relative_model_matrix;
    y0_ = (float)transformation_matrix_.t(1);
}

void Wheel::set_updated_relative_model_matrix(const TransformationMatrix<float, ScenePos, 3>& relative_model_matrix)
{
    transformation_matrix_.t = relative_model_matrix.t;
}

void Wheel::set_absolute_model_matrix(const TransformationMatrix<float, ScenePos, 3>& absolute_model_matrix)
{
    // do nothing
}

TransformationMatrix<float, ScenePos, 3> Wheel::get_new_relative_model_matrix() const
{
    return transformation_matrix_;
}

void Wheel::advance_time(float dt, const StaticWorld& world) {
    FixedArray<float, 3> tire_angles{fixed_zeros<float, 3>()};
    if (auto t = rigid_body_.tires_.try_get(tire_id_); t != nullptr) {
        tire_angles(1) = t->angle_y;
        transformation_matrix_.t(1) = y0_ + t->shock_absorber_position;
        angle_x_ = t->angle_x;
    }
    tire_angles(0) = angle_x_;
    transformation_matrix_.R = tait_bryan_angles_2_matrix(tire_angles);
}

void Wheel::notify_destroyed(SceneNode& destroyed_object) {
    if (destroyed_object.has_relative_movable()) {
        if (&destroyed_object.get_relative_movable() != this) {
            verbose_abort("Unexpected relative movable");
        }
        destroyed_object.clear_relative_movable();
    }
    global_object_pool.remove(this);
}
