// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Triangle_3D.hpp"
#include <VehicleArena/Geometry/Colored_Vertex.hpp>
#include <VehicleArena/Geometry/Primitives/Axis_Aligned_Bounding_Box.hpp>
#include <VehicleArena/Geometry/Primitives/Convex_Polygon.hpp>
#include <VehicleArena/Geometry/Primitives/Plane_Nd.hpp>
#include <VehicleArena/Geometry/Welzl.hpp>
#include <VehicleArena/Math/Transformation/Transformation_Matrix.hpp>
#include <VehicleArena/Scene_Config/Scene_Precision.hpp>

using namespace VA;

template <class TPos>
Triangle3D<TPos>::Triangle3D(const FixedArray<TPos, 3, 3>& vertices)
    : vertices_{ vertices }
{}

template <class TPos>
template <class TPos2>
Triangle3D<TPos>::Triangle3D(const FixedArray<ColoredVertex<TPos2>, 3>& vertices)
    : vertices_{
        vertices(0).position.template casted<TPos>(),
        vertices(1).position.template casted<TPos>(),
        vertices(2).position.template casted<TPos>()}
{}

template <class TPos>
template <class TPos2, class TPosTransform>
Triangle3D<TPos>::Triangle3D(
    const FixedArray<ColoredVertex<TPos2>, 3>& vertices,
    const TransformationMatrix<float, TPosTransform, 3>& transformation)
    : vertices_{
        transformation.transform(vertices(0).position.template casted<TPosTransform>()).template casted<TPos>(),
        transformation.transform(vertices(1).position.template casted<TPosTransform>()).template casted<TPos>(),
        transformation.transform(vertices(2).position.template casted<TPosTransform>()).template casted<TPos>()}
{}

template <class TPos>
const FixedArray<TPos, 3, 3>& Triangle3D<TPos>::vertices() const {
    return vertices_;
}

template <class TPos>
ConvexPolygon3D<typename Triangle3D<TPos>::I, TPos, 3> Triangle3D<TPos>::polygon() const {
    return ConvexPolygon3D<typename Triangle3D<TPos>::I, TPos, 3>{ vertices_ };
}

template <class TPos>
BoundingSphere<TPos, 3> Triangle3D<TPos>::bounding_sphere(std::minstd_rand& rng) const {
    return welzl_from_fixed(funpack(vertices_), rng).template casted<TPos>();
}

template <class TPos>
AxisAlignedBoundingBox<TPos, 3> Triangle3D<TPos>::aabb() const {
    return AxisAlignedBoundingBox<TPos, 3>::from_points(vertices_);
}

namespace VA {

template class Triangle3D<float>;
template class Triangle3D<CompressedScenePos>;

template Triangle3D<CompressedScenePos>::Triangle3D(
    const FixedArray<ColoredVertex<float>, 3>& vertices);
template Triangle3D<CompressedScenePos>::Triangle3D(
    const FixedArray<ColoredVertex<CompressedScenePos>, 3>& vertices);
template Triangle3D<CompressedScenePos>::Triangle3D(
    const FixedArray<ColoredVertex<float>, 3>& vertices,
    const TransformationMatrix<float, ScenePos, 3>& transformation);
template Triangle3D<CompressedScenePos>::Triangle3D(
    const FixedArray<ColoredVertex<CompressedScenePos>, 3>& vertices,
    const TransformationMatrix<float, ScenePos, 3>& transformation);
template Triangle3D<float>::Triangle3D(
    const FixedArray<ColoredVertex<float>, 3>& vertices,
    const TransformationMatrix<float, float, 3>& transformation);

}
