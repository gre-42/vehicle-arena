// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Geometry/Billboard_Id.hpp>
#include <VehicleArena/Scene_Config/Scene_Precision.hpp>
#include <cstddef>
#include <list>
#include <map>
#include <memory>
#include <set>

namespace VA {

struct TransformedColoredVertexArray;
template <typename TData, size_t... tshape>
class FixedArray;
template <class TDir, class TPos, size_t n>
class TransformationMatrix;
template <class TPos>
class ColoredVertexArray;
struct SceneGraphConfig;
enum class ExternalRenderPassType;

enum class InvisibilityHandling {
    RAISE,
    SKIP
};

class LargeInstancesQueue {
public:
    explicit LargeInstancesQueue(ExternalRenderPassType render_pass);
    ~LargeInstancesQueue();
    void insert(
        const std::list<std::shared_ptr<ColoredVertexArray<float>>>& scvas,
        const FixedArray<ScenePos, 4, 4>& mvp,
        const TransformationMatrix<float, ScenePos, 3>& m,
        const FixedArray<ScenePos, 3>& offset,
        BillboardId billboard_id,
        const SceneGraphConfig& scene_graph_config,
        InvisibilityHandling invisibility_handling);
    const std::list<TransformedColoredVertexArray>& queue() const;
    ExternalRenderPassType render_pass() const;
private:
    ExternalRenderPassType render_pass_;
    std::list<TransformedColoredVertexArray> queue_;
};

}
