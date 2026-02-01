// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Relative_Transformer.hpp"
#include <VehicleArena/Geometry/Coordinates/Homogeneous.hpp>
#include <VehicleArena/Math/Fixed_Rodrigues.hpp>
#include <VehicleArena/Memory/Dangling_Unique_Ptr.hpp>
#include <VehicleArena/Memory/Object_Pool.hpp>
#include <VehicleArena/Scene_Graph/Elements/Scene_Node.hpp>

using namespace VA;

RelativeTransformer::RelativeTransformer(
    const FixedArray<float, 3>& v,
    const FixedArray<float, 3>& w)
    : transformation_matrix_{
          fixed_nans<float, 3, 3>(),
          fixed_nans<ScenePos, 3>()
    }
    , v_{ v }
    , w_{ w }
{}

RelativeTransformer::~RelativeTransformer() {
    on_destroy.clear();
}

void RelativeTransformer::set_initial_relative_model_matrix(const TransformationMatrix<float, ScenePos, 3>& relative_model_matrix)
{
    transformation_matrix_ = relative_model_matrix;
}

void RelativeTransformer::set_updated_relative_model_matrix(const TransformationMatrix<float, ScenePos, 3>& relative_model_matrix)
{
    transformation_matrix_.t = relative_model_matrix.t;
}

void RelativeTransformer::set_absolute_model_matrix(const TransformationMatrix<float, ScenePos, 3>& absolute_model_matrix)
{
    // do nothing
}

TransformationMatrix<float, ScenePos, 3> RelativeTransformer::get_new_relative_model_matrix() const
{
    return transformation_matrix_;
}

void RelativeTransformer::advance_time(float dt, const StaticWorld& world) {
    transformation_matrix_.t += (dt * v_).casted<ScenePos>();
    transformation_matrix_.R = dot2d(rodrigues1(dt * w_), transformation_matrix_.R);
}

void RelativeTransformer::notify_destroyed(SceneNode& destroyed_object) {
    if (destroyed_object.has_relative_movable()) {
        if (&destroyed_object.get_relative_movable() != this) {
            verbose_abort("Unexpected relative movable");
        }
        destroyed_object.clear_relative_movable();
    }
    global_object_pool.remove(this);
}
