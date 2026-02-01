// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Scene_Config/Scene_Precision.hpp>
#include <cstddef>
#include <memory>

namespace VA {

template <class TDir, class TPos, size_t n>
class TransformationMatrix;
enum class RenderingDynamics;
class SceneNode;
template <class T>
class DanglingUniquePtr;
class Renderable;

class IInstantiationReference {
public:
    virtual void add_renderable(
        const std::string& name,
        const std::shared_ptr<const Renderable>& renderable) = 0;
    virtual TransformationMatrix<float, ScenePos, 3> absolute_model_matrix() const = 0;
    virtual void auto_add_root_node(
        const std::string& name,
        std::unique_ptr<SceneNode>&& scene_node,
        RenderingDynamics rendering_dynamics) = 0;
};

}
