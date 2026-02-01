// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Collision_Edges.hpp"
#include <VehicleArena/Geometry/Fixed_Cross.hpp>
#include <VehicleArena/Math/Orderable_Fixed_Array.hpp>

using namespace VA;

std::pair<OrderableFixedArray<CompressedScenePos, 3>, OrderableFixedArray<CompressedScenePos, 3>>
    OrderableEdgeSphere::key() const
{
    if (make_orderable(collision_line_sphere.line[0]) > make_orderable(collision_line_sphere.line[1])) {
        return std::make_pair(
            make_orderable(collision_line_sphere.line[0]),
            make_orderable(collision_line_sphere.line[1]));
    } else {
        return std::make_pair(
            make_orderable(collision_line_sphere.line[1]),
            make_orderable(collision_line_sphere.line[0]));
    }
}

bool OrderableEdgeSphere::operator < (const OrderableEdgeSphere& other) const {
    return key() < other.key();
}

CollisionEdges::CollisionEdges() = default;

CollisionEdges::~CollisionEdges() = default;

template <size_t tnvertices>
void CollisionEdges::insert(
    const FixedArray<CompressedScenePos, tnvertices, 3>& tri,
    PhysicsMaterial physics_material)
{
    for (size_t i = 0; i < tnvertices; ++i) {
        insert(tri[i], tri[(i + 1) % tnvertices], physics_material);
    }
}

void CollisionEdges::insert(
    const FixedArray<CompressedScenePos, 3>& a,
    const FixedArray<CompressedScenePos, 3>& b,
    PhysicsMaterial physics_material)
{
    OrderableEdgeSphere edge{
        .collision_line_sphere{
            .bounding_sphere{BoundingSphere<CompressedScenePos, 3>{FixedArray<CompressedScenePos, 2, 3>{a, b}}},
            .physics_material = physics_material,
            .line{a, b},
            .ray{a, b}} };
    edges_.insert(edge);
}

CollisionEdges::const_iterator CollisionEdges::begin() const {
    return edges_.begin();
}

CollisionEdges::const_iterator CollisionEdges::end() const {
    return edges_.end();
}

size_t CollisionEdges::size() const {
    return edges_.size();
}

namespace VA {
    template void CollisionEdges::insert<3>(
        const FixedArray<CompressedScenePos, 3, 3>& tri,
        PhysicsMaterial physics_material);
    template void CollisionEdges::insert<4>(
        const FixedArray<CompressedScenePos, 4, 3>& tri,
        PhysicsMaterial physics_material);
}
