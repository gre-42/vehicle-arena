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

template <class ScenePos, size_t tndim>
class BoundingSphere;
template <class ScenePos, size_t tndim>
class AxisAlignedBoundingBox;
template <class TPosition, size_t tnvertices>
struct CollisionPolygonSphere;
template <class TPosition>
struct CollisionRidgeSphere;
template <class TPosition>
struct CollisionLineSphere;
template <typename ScenePos, size_t... tshape>
class FixedArray;
template <class TDir, class TPos, size_t n>
class TransformationMatrix;

class IIntersectable {
public:
    virtual ~IIntersectable() = default;
    virtual BoundingSphere<CompressedScenePos, 3> bounding_sphere() const = 0;
    virtual AxisAlignedBoundingBox<CompressedScenePos, 3> aabb() const = 0;
    virtual std::shared_ptr<IIntersectable> sweep(
        const AxisAlignedBoundingBox<CompressedScenePos, 3>& aabb) const = 0;
    virtual bool touches(
        const CollisionPolygonSphere<CompressedScenePos, 4>& q,
        ScenePos& overlap,
        FixedArray<ScenePos, 3>& intersection_point,
        FixedArray<SceneDir, 3>& normal) const = 0;
    virtual bool touches(
        const CollisionPolygonSphere<CompressedScenePos, 3>& t,
        ScenePos& overlap,
        FixedArray<ScenePos, 3>& intersection_point,
        FixedArray<SceneDir, 3>& normal) const = 0;
    virtual bool touches(
        const CollisionRidgeSphere<CompressedScenePos>& r1,
        ScenePos& overlap,
        FixedArray<ScenePos, 3>& intersection_point,
        FixedArray<SceneDir, 3>& normal) const = 0;
    virtual bool touches(
        const CollisionLineSphere<CompressedScenePos>& l1,
        ScenePos& overlap,
        ScenePos& ray_t,
        FixedArray<ScenePos, 3>& intersection_point,
        FixedArray<SceneDir, 3>& normal) const = 0;
    virtual bool touches(
        const IIntersectable& intersectable,
        ScenePos& overlap,
        FixedArray<ScenePos, 3>& intersection_point,
        FixedArray<SceneDir, 3>& normal) const = 0;
    virtual bool touches(
        const IIntersectable& intersectable,
        const TransformationMatrix<SceneDir, ScenePos, 3>& trafo,
        ScenePos& overlap,
        FixedArray<ScenePos, 3>& intersection_point,
        FixedArray<SceneDir, 3>& normal) const = 0;
    virtual bool can_spawn_at(
        const CollisionPolygonSphere<CompressedScenePos, 3>& t) const = 0;
    virtual bool can_spawn_at(
        const CollisionPolygonSphere<CompressedScenePos, 4>& q) const = 0;
    virtual bool can_spawn_at(
        const IIntersectable& intersectable) const = 0;
    virtual bool can_spawn_at(
        const IIntersectable& intersectable,
        const TransformationMatrix<SceneDir, ScenePos, 3>& trafo) const = 0;
};

}
