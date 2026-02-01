// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Look_At_Movable.hpp"
#include <VehicleArena/Geometry/Coordinates/Gl_Look_At.hpp>
#include <VehicleArena/Geometry/Coordinates/Homogeneous.hpp>
#include <VehicleArena/Memory/Object_Pool.hpp>
#include <VehicleArena/Physics/Containers/Advance_Times.hpp>
#include <VehicleArena/Scene_Graph/Containers/Scene.hpp>
#include <VehicleArena/Scene_Graph/Elements/Scene_Node.hpp>
#include <VehicleArena/Threads/Throwing_Lock_Guard.hpp>

using namespace VA;

LookAtMovable::LookAtMovable(
    AdvanceTimes& advance_times,
    Scene& scene,
    VariableAndHash<std::string> follower_name,
    DanglingBaseClassRef<SceneNode> follower_node,
    DanglingBaseClassRef<SceneNode> followed_node,
    IAbsoluteMovable& followed)
    : follower_setter{ *this }
    , followed_setter{ *this }
    , advance_times_{ advance_times }
    , scene_{ scene }
    , follower_name_{ std::move(follower_name) }
    , follower_node_{ follower_node.ptr() }
    , followed_node_{ followed_node.ptr() }
    , followed_{ &followed }
    , transformation_matrix_{ fixed_nans<float, 3, 3>(), fixed_nans<ScenePos, 3>() }
{}

LookAtMovable::~LookAtMovable() {
    on_destroy.clear();
}

void LookAtMovable::advance_time(float dt, const StaticWorld& world) {
    if (followed_ == nullptr) {
        return;
    }
    auto dmat = followed_->get_new_absolute_model_matrix();
    const auto& dpos = dmat.t;
    auto R = gl_lookat_absolute(transformation_matrix_.t, dpos);
    if (R.has_value()) {
        transformation_matrix_.R = R->casted<float>();
    }
}

void LookAtMovable::set_absolute_model_matrix(const TransformationMatrix<float, ScenePos, 3>& absolute_model_matrix) {
    transformation_matrix_ = absolute_model_matrix;
}

TransformationMatrix<float, ScenePos, 3> LookAtMovable::get_new_absolute_model_matrix() const {
    return transformation_matrix_;
}

void LookAtMovable::notify_destroyed(SceneNode& destroyed_object) {
    ThrowingLockGuard lock{scene_.delete_node_mutex};
    if ((follower_node_ == nullptr) != (followed_node_ == nullptr)) {
        verbose_abort("LookAtMovable in inconsistent state");
    }
    if (follower_node_ == nullptr) {
        return;
    }
    if (&destroyed_object == followed_node_.get()) {
        if (follower_node_->shutdown_phase() == ShutdownPhase::NONE) {
            follower_node_ = nullptr;
            scene_.delete_root_node(follower_name_);
        }
    } else if (follower_node_->has_absolute_movable()) {
        if (&follower_node_->get_absolute_movable() != this) {
            verbose_abort("Unexpected absolute movable");
        }
        follower_node_->clear_absolute_movable();
    }
    follower_node_ = nullptr;
    followed_node_ = nullptr;
    global_object_pool.remove(this);
}

LookAtMovableFollowerNodeSetter::LookAtMovableFollowerNodeSetter(LookAtMovable& look_at_movable)
    : look_at_movable_{ look_at_movable }
    , removal_tokens_{ nullptr, CURRENT_SOURCE_LOCATION }
{}

void LookAtMovableFollowerNodeSetter::set_scene_node(
    Scene& scene,
    const DanglingBaseClassRef<SceneNode>& node,
    VariableAndHash<std::string> node_name,
    SourceLocation loc)
{
    removal_tokens_.set(node->on_clear, loc);
    removal_tokens_.add([this, node](){
        look_at_movable_.notify_destroyed(node.get());
    }, loc);
    node->set_absolute_movable({ look_at_movable_, loc });
}

LookAtMovableFollowedNodeSetter::LookAtMovableFollowedNodeSetter(LookAtMovable& look_at_movable)
    : look_at_movable_{ look_at_movable }
    , removal_tokens_{ nullptr, CURRENT_SOURCE_LOCATION }
{}

void LookAtMovableFollowedNodeSetter::set_scene_node(
    Scene& scene,
    const DanglingBaseClassRef<SceneNode>& node,
    VariableAndHash<std::string> node_name,
    SourceLocation loc)
{
    removal_tokens_.set(node->on_clear, loc);
    removal_tokens_.add([this, node](){
        look_at_movable_.notify_destroyed(node.get());
    }, loc);
}
