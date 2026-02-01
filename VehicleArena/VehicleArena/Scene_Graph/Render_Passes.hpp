// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Hashing/Variable_And_Hash.hpp>
#include <VehicleArena/Memory/Dangling_Base_Class.hpp>
#include <VehicleArena/Memory/Dangling_Unique_Ptr.hpp>
#include <VehicleArena/Scene_Graph/Remote_User_Filter.hpp>
#include <chrono>
#include <compare>
#include <cstdint>

namespace VA {

enum class ExternalRenderPassType;
class SceneNode;
class IRenderableScene;

struct ExternalRenderPass {
    RemoteObserver observer = RemoteObserver::all();
    ExternalRenderPassType pass;
    std::chrono::steady_clock::time_point time;
    VariableAndHash<std::string> black_node_name;
    DanglingBaseClassPtr<SceneNode> singular_node = nullptr;
    DanglingBaseClassPtr<SceneNode> nonstandard_camera_node = nullptr;
    DanglingBaseClassPtr<IRenderableScene> renderable_scene = nullptr;
    std::strong_ordering operator <=> (const ExternalRenderPass&) const = default;
};

enum class InternalRenderPass {
    NONE = 0,
    INITIAL = 1 << 0,
    BLENDED_EARLY = 1 << 1,
    BLENDED_LATE = 1 << 2,
    AGGREGATE = 1 << 3,
    PARTICLES = 1 << 4,
    PRELOADED = AGGREGATE | PARTICLES,
    ANY_BLENDED = BLENDED_EARLY | BLENDED_LATE
};

inline InternalRenderPass operator&(InternalRenderPass a, InternalRenderPass b) {
    return InternalRenderPass((int)a & (int)b);
}

inline bool any(InternalRenderPass a) {
    return a != InternalRenderPass::NONE;
}

}
