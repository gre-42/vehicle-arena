// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Misc/Features.hpp>
#include <VehicleArena/Threads/Recursive_Shared_Mutex.hpp>
#include <chrono>
#include <list>
#include <mutex>

namespace VA {

template <class T>
class VariableAndHash;
class SceneNodeResources;
class RenderingResources;
class Scene;
class PhysicsEngine;
struct PhysicsEngineConfig;
class SetFps;
class BaseLog;
class DeleteRigidBodyMutex;
class PhysicsLoop;
struct Beacon;
class DynamicWorld;
template <class TTimepoint>
class TimeAndPause;

class PhysicsIteration {
    friend PhysicsLoop;
public:
    PhysicsIteration(
        SceneNodeResources& scene_node_resources,
        RenderingResources& rendering_resources,
        Scene& scene,
        DynamicWorld& dynamic_world,
        PhysicsEngine& physics_engine,
        std::function<void(const TimeAndPause<std::chrono::steady_clock::time_point>&)> send_and_receive,
        SafeAtomicRecursiveSharedMutex& delete_node_mutex,
        const PhysicsEngineConfig& physics_cfg,
        BaseLog* base_log = nullptr);
    ~PhysicsIteration();
    void operator()(const TimeAndPause<std::chrono::steady_clock::time_point>& time);
private:
    SceneNodeResources& scene_node_resources_;
    RenderingResources& rendering_resources_;
    Scene& scene_;
    DynamicWorld& dynamic_world_;
    PhysicsEngine& physics_engine_;
    std::function<void(const TimeAndPause<std::chrono::steady_clock::time_point>&)> send_and_receive_;
    SafeAtomicRecursiveSharedMutex& delete_node_mutex_;
    const PhysicsEngineConfig& physics_cfg_;
    BaseLog* base_log_;
    std::list<VariableAndHash<std::string>> beacon_nodes_;
};

}
