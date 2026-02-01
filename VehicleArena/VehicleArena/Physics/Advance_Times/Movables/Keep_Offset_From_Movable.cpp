// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Keep_Offset_From_Movable.hpp"
#include <VehicleArena/Memory/Object_Pool.hpp>
#include <VehicleArena/Physics/Containers/Advance_Times.hpp>
#include <VehicleArena/Scene_Graph/Containers/Scene.hpp>
#include <VehicleArena/Scene_Graph/Elements/Scene_Node.hpp>

using namespace VA;

KeepOffsetFromMovable::KeepOffsetFromMovable(
    AdvanceTimes& advance_times,
    Scene& scene,
    VariableAndHash<std::string> follower_name,
    DanglingBaseClassRef<SceneNode> followed_node,
    IAbsoluteMovable& followed,
    const FixedArray<float, 3>& offset)
    : set_follower{ *this }
    , set_followed{ *this }
    , advance_times_{ advance_times }
    , scene_{ scene }
    , follower_name_{ std::move(follower_name) }
    , followed_node_{ followed_node.ptr() }
    , followed_{ &followed }
    , offset_{ offset }
    , transformation_matrix_{ fixed_nans<float, 3, 3>(), fixed_nans<ScenePos, 3>() }
{}

KeepOffsetFromMovable::~KeepOffsetFromMovable()
{}

void KeepOffsetFromMovable::advance_time(float dt, const StaticWorld& world) {
    if (followed_ == nullptr) {
        return;
    }
    transformation_matrix_.t = followed_->get_new_absolute_model_matrix().t + offset_.casted<ScenePos>();
}

void KeepOffsetFromMovable::set_absolute_model_matrix(const TransformationMatrix<float, ScenePos, 3>& absolute_model_matrix) {
    transformation_matrix_ = absolute_model_matrix;
}

TransformationMatrix<float, ScenePos, 3> KeepOffsetFromMovable::get_new_absolute_model_matrix() const {
    return transformation_matrix_;
}

void KeepOffsetFromMovable::notify_destroyed(SceneNode& destroyed_object) {
    if (&destroyed_object == followed_node_.get()) {
        followed_node_ = nullptr;
        followed_ = nullptr;
        if (!follower_name_->empty()) {
            VariableAndHash<std::string> fn = follower_name_;
            follower_name_ = VariableAndHash<std::string>();
            scene_.delete_root_node(fn);
        }
    } else {
        follower_name_ = VariableAndHash<std::string>();
        global_object_pool.remove(this);
    }
}

KeepOffsetFromMovableFollowerNodeSetter::KeepOffsetFromMovableFollowerNodeSetter(KeepOffsetFromMovable& keep_offset)
    : keep_offset_{ keep_offset }
    , removal_tokens_{ nullptr, CURRENT_SOURCE_LOCATION }
{}

void KeepOffsetFromMovableFollowerNodeSetter::set_scene_node(
    Scene& scene,
    const DanglingBaseClassRef<SceneNode>& node,
    VariableAndHash<std::string> node_name,
    SourceLocation loc)
{
    removal_tokens_.set(node->on_clear, loc);
    removal_tokens_.add([this, node](){
        keep_offset_.notify_destroyed(node.get());
    }, loc);
    node->set_absolute_movable({ keep_offset_, loc });
}

KeepOffsetFromMovableFollowedNodeSetter::KeepOffsetFromMovableFollowedNodeSetter(KeepOffsetFromMovable& keep_offset)
    : keep_offset_{ keep_offset }
    , removal_tokens_{ nullptr, CURRENT_SOURCE_LOCATION }
{}

void KeepOffsetFromMovableFollowedNodeSetter::set_scene_node(
    Scene& scene,
    const DanglingBaseClassRef<SceneNode>& node,
    VariableAndHash<std::string> node_name,
    SourceLocation loc)
{
    removal_tokens_.set(node->on_clear, loc);
    removal_tokens_.add([this, node](){
        keep_offset_.notify_destroyed(node.get());
    }, loc);
}
