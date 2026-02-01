// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Geometry/Mesh/Typed_Mesh.hpp>
#include <VehicleArena/Geometry/Primitives/Bvh.hpp>
#include <VehicleArena/Geometry/Primitives/Bvh_Grid.hpp>
#include <VehicleArena/Physics/Containers/Elements/Collision_Triangle_Sphere.hpp>
#include <VehicleArena/Scene_Config/Scene_Precision.hpp>
#include <VehicleArena/Scene_Graph/Scene_Element_Type.hpp>
#include <memory>
#include <set>

namespace VA {

class Camera;
class IRenderable;
struct DeferredRenderable;
struct RenderEngineConfig;
struct PhysicsEngineConfig;
class RigidBodyVehicle;
class IIntersectableMesh;

struct RigidBodyAndIntersectableMesh {
    std::shared_ptr<RigidBodyVehicle> rb;
    TypedMesh<std::shared_ptr<IIntersectableMesh>> mesh;
};

class SceneGraph {
public:
    explicit SceneGraph(
        const RenderEngineConfig& rcfg,
        const PhysicsEngineConfig& pcfg);
    ~SceneGraph();
    void render(const Camera& camera);
    void move(SceneElementTypes types);
private:
    using RenderablesBvh = Bvh<
        CompressedScenePos,
        3,
        std::shared_ptr<IRenderable>>;
    using CollidablePolygonBvh = CompressedBvhGrid<
        CompressedScenePos,
        HalfCompressedScenePos,
        RigidBodyAndCollisionTriangleSphere<CompressedScenePos>,
        RigidBodyAndCollisionTriangleSphere<HalfCompressedScenePos>,
        3>;
    using CollidableMovablesBvh = BvhGrid<
        CompressedScenePos,
        3,
        RigidBodyAndIntersectableMesh>;
    void render(
        const FixedArray<float, 4, 4>& vp,
        const FixedArray<ScenePos, 3>& offset,
        std::set<DeferredRenderable>& deferred,
        SceneElementTypes types);
    RenderablesBvh dynamic_renderables_;
    RenderablesBvh static_renderables_;
    CollidableMovablesBvh dynamic_object_collidables_;
    CollidablePolygonBvh static_polygonal_collidables_;
};

}
