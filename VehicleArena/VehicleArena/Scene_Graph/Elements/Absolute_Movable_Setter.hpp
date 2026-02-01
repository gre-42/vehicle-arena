// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Hashing/Variable_And_Hash.hpp>
#include <VehicleArena/Memory/Dangling_Unique_Ptr.hpp>
#include <VehicleArena/Memory/Object_Pool.hpp>
#include <VehicleArena/Memory/Optional_Cast.hpp>
#include <VehicleArena/Scene_Graph/Elements/Scene_Node.hpp>
#include <VehicleArena/Scene_Graph/Interfaces/INode_Setter.hpp>

namespace VA {

class Scene;

template <class TAbsoluteMovable>
class AbsoluteMovableSetter {
    AbsoluteMovableSetter(const AbsoluteMovableSetter&) = delete;
    AbsoluteMovableSetter& operator = (const AbsoluteMovableSetter&) = delete;
public:
    AbsoluteMovableSetter(
        Scene& scene,
        DanglingBaseClassRef<SceneNode> node,
        VariableAndHash<std::string> node_name,
        std::unique_ptr<TAbsoluteMovable, DeleteFromPool<TAbsoluteMovable>>&& absolute_movable,
        DanglingBaseClassPtr<INodeSetter> node_setter,
        SourceLocation loc)
        : absolute_movable{ nullptr, nullptr }
        , scene_{ &scene }
        , node_name_{ std::move(node_name) }
        , absolute_movable_ptr_{ *absolute_movable, CURRENT_SOURCE_LOCATION }
        , node_setter_{ std::move(node_setter) }
        , node_{ node }
        , lock_{ node->mutex_ }
        , loc_{ loc }
    {
        if (node->absolute_movable_ != nullptr) {
            throw std::runtime_error("Absolute movable already set (0)");
        }
        absolute_movable->set_absolute_model_matrix(node->absolute_model_matrix());
        // Initialize after check above so the absolute_movable unique_ptr is not
        // destroyed in case of an error.
        this->absolute_movable = std::move(absolute_movable);
    }
    AbsoluteMovableSetter(
        DanglingBaseClassRef<SceneNode> node,
        std::unique_ptr<TAbsoluteMovable, DeleteFromPool<TAbsoluteMovable>>&& absolute_movable,
        SourceLocation loc)
        : absolute_movable{ nullptr, nullptr }
        , scene_{ nullptr }
        , absolute_movable_ptr_{ *absolute_movable, CURRENT_SOURCE_LOCATION }
        , node_setter_{ nullptr }
        , node_{ node }
        , lock_{ node->mutex_ }
        , loc_{ loc }
    {
        if (node->absolute_movable_ != nullptr) {
            throw std::runtime_error("Absolute movable already set (1)");
        }
        absolute_movable->set_absolute_model_matrix(node->absolute_model_matrix());
        // Initialize after check above so the absolute_movable unique_ptr is not
        // destroyed in case of an error.
        this->absolute_movable = std::move(absolute_movable);
    }
    AbsoluteMovableSetter(
        Scene& scene,
        DanglingBaseClassRef<SceneNode> node,
        VariableAndHash<std::string> node_name,
        std::unique_ptr<TAbsoluteMovable, DeleteFromPool<TAbsoluteMovable>>&& absolute_movable,
        SourceLocation loc)
        : AbsoluteMovableSetter{
            scene,
            std::move(node),
            std::move(node_name),
            std::move(absolute_movable),
            { *absolute_movable, loc },
            loc }
    {}
    AbsoluteMovableSetter(
        DanglingBaseClassRef<SceneNode> node,
        DanglingBaseClassRef<TAbsoluteMovable> absolute_movable,
        SourceLocation loc)
        : absolute_movable_ptr_{ absolute_movable.ptr() }
        , scene_{ nullptr }
        , node_setter_{ nullptr }
        , node_{ node }
        , lock_{ node->mutex_ }
        , loc_{ loc }
    {}
    ~AbsoluteMovableSetter() {
        if (absolute_movable == nullptr) {
            if (node_setter_ != nullptr) {
                if (scene_ == nullptr) {
                    verbose_abort("~AbsoluteMovableSetter: Scene not set");
                }
                node_setter_->set_scene_node(
                    *scene_,
                    node_,
                    node_name_,
                    CURRENT_SOURCE_LOCATION);
            }
        }
    }
    std::unique_ptr<TAbsoluteMovable, DeleteFromPool<TAbsoluteMovable>> absolute_movable;
private:
    Scene* scene_;
    VariableAndHash<std::string> node_name_;
    DanglingBaseClassPtr<IAbsoluteMovable> absolute_movable_ptr_;
    DanglingBaseClassPtr<INodeSetter> node_setter_;
    DanglingBaseClassRef<SceneNode> node_;
    std::scoped_lock<SafeAtomicRecursiveSharedMutex> lock_;
    SourceLocation loc_;
};

}
