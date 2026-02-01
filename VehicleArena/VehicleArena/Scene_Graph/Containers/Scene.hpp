// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Hashing/Variable_And_Hash.hpp>
#include <VehicleArena/Memory/Dangling_Base_Class.hpp>
#include <VehicleArena/Threads/Fast_Mutex.hpp>
#include <VehicleArena/Threads/Recursive_Shared_Mutex.hpp>
#include <chrono>
#include <memory>
#include <string>

namespace VA {

enum class RenderingDynamics;
enum class RenderingStrategies;
class SceneNode;

class Scene {
public:
    void add_to_trash_can(std::unique_ptr<SceneNode>&& node);
    void add_to_trash_can(std::unique_ptr<DanglingBaseClass>&& obj);
    void delete_root_node(const VariableAndHash<std::string>& name);
    void unregister_node(const VariableAndHash<std::string>& name);
    void auto_add_root_node(
        const VariableAndHash<std::string>& name,
        std::unique_ptr<SceneNode>&& scene_node,
        RenderingDynamics rendering_dynamics);
    void add_root_node(
        const VariableAndHash<std::string>& name,
        std::unique_ptr<SceneNode>&& scene_node,
        RenderingDynamics rendering_dynamics,
        RenderingStrategies rendering_strategy);
    void register_node(
        const VariableAndHash<std::string>& name,
        const DanglingBaseClassRef<SceneNode>& scene_node);
    DanglingBaseClassRef<SceneNode> get_node(
        const VariableAndHash<std::string>& name,
        SourceLocation loc) const;
    void move(float dt, std::chrono::steady_clock::time_point time);
    void notify_cleanup_required();
    void notify_cleanup_done();
    bool shutting_down() const;
    size_t get_uuid();
    std::string get_temporary_instance_suffix();
    SafeAtomicRecursiveSharedMutex delete_node_mutex;
    SafeAtomicRecursiveSharedMutex render_mutex;
};

}
