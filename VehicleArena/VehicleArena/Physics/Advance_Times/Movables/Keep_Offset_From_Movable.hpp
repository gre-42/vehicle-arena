// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Array/Fixed_Array.hpp>
#include <VehicleArena/Hashing/Variable_And_Hash.hpp>
#include <VehicleArena/Math/Transformation/Transformation_Matrix.hpp>
#include <VehicleArena/Memory/Dangling_Base_Class.hpp>
#include <VehicleArena/Memory/Dangling_Unique_Ptr.hpp>
#include <VehicleArena/Memory/Destruction_Functions.hpp>
#include <VehicleArena/Physics/Interfaces/IAdvance_Time.hpp>
#include <VehicleArena/Scene_Graph/Interfaces/INode_Setter.hpp>
#include <VehicleArena/Scene_Graph/Interfaces/Scene_Node/IAbsolute_Movable.hpp>
#include <memory>

namespace VA {

class AdvanceTimes;
class SceneNode;
class Scene;
class KeepOffsetFromMovable;

class KeepOffsetFromMovableFollowerNodeSetter: public INodeSetter {
public:
    explicit KeepOffsetFromMovableFollowerNodeSetter(KeepOffsetFromMovable& keep_offset);
    virtual void set_scene_node(
        Scene& scene,
        const DanglingBaseClassRef<SceneNode>& node,
        VariableAndHash<std::string> node_name,
        SourceLocation loc) override;
private:
    KeepOffsetFromMovable& keep_offset_;
    DestructionFunctionsRemovalTokens removal_tokens_;
};

class KeepOffsetFromMovableFollowedNodeSetter: public INodeSetter {
public:
    explicit KeepOffsetFromMovableFollowedNodeSetter(KeepOffsetFromMovable& keep_offset);
    virtual void set_scene_node(
        Scene& scene,
        const DanglingBaseClassRef<SceneNode>& node,
        VariableAndHash<std::string> node_name,
        SourceLocation loc) override;
private:
    KeepOffsetFromMovable& keep_offset_;
    DestructionFunctionsRemovalTokens removal_tokens_;
};

class KeepOffsetFromMovable: public IAbsoluteMovable, public IAdvanceTime, public virtual DanglingBaseClass {
    friend KeepOffsetFromMovableFollowerNodeSetter;
    friend KeepOffsetFromMovableFollowedNodeSetter;
public:
    KeepOffsetFromMovable(
        AdvanceTimes& advance_times,
        Scene& scene,
        VariableAndHash<std::string> follower_name,
        DanglingBaseClassRef<SceneNode> followed_node,
        IAbsoluteMovable& followed,
        const FixedArray<float, 3>& offset);
    virtual ~KeepOffsetFromMovable() override;
    virtual void advance_time(float dt, const StaticWorld& world) override;
    virtual void set_absolute_model_matrix(const TransformationMatrix<float, ScenePos, 3>& absolute_model_matrix) override;
    virtual TransformationMatrix<float, ScenePos, 3> get_new_absolute_model_matrix() const override;
    KeepOffsetFromMovableFollowerNodeSetter set_follower;
    KeepOffsetFromMovableFollowedNodeSetter set_followed;

private:
    void notify_destroyed(SceneNode& destroyed_object);
    AdvanceTimes& advance_times_;
    Scene& scene_;
    VariableAndHash<std::string> follower_name_;
    DanglingBaseClassPtr<SceneNode> followed_node_;
    IAbsoluteMovable* followed_;
    FixedArray<float, 3> offset_;
    TransformationMatrix<float, ScenePos, 3> transformation_matrix_;
};

}
