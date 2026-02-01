// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Barrier_Triangle_Hitbox.hpp"
#include <VehicleArena/Geometry/Exceptions/Triangle_Exception.hpp>
#include <VehicleArena/Geometry/Mesh/Colored_Vertex_Array.hpp>
#include <VehicleArena/Geometry/Mesh/Contour.hpp>
#include <VehicleArena/Geometry/Mesh/Vertex_Normals.hpp>
#include <VehicleArena/Geometry/Physics_Material.hpp>
#include <VehicleArena/Geometry/Triangle_Normal.hpp>
#include <VehicleArena/Math/Fixed_Math.hpp>
#include <VehicleArena/Math/Orderable_Fixed_Array.hpp>
#include <VehicleArena/Scene_Config/Scene_Precision.hpp>

using namespace VA;

template <class TPos>
UUVector<FixedArray<TPos, 3, 3>> VA::barrier_triangle_hitbox(
    const FixedArray<TPos, 3>& am,
    const FixedArray<TPos, 3>& bm,
    const FixedArray<TPos, 3>& cm,
    const FixedArray<float, 3>& half_width_a,
    const FixedArray<float, 3>& half_width_b,
    const FixedArray<float, 3>& half_width_c,
    bool ab_is_contour_edge,
    bool bc_is_contour_edge,
    bool ca_is_contour_edge)
{
    FixedArray<TPos, 3> A = am + half_width_a.casted<TPos>();
    FixedArray<TPos, 3> B = bm + half_width_b.casted<TPos>();
    FixedArray<TPos, 3> C = cm + half_width_c.casted<TPos>();
    FixedArray<TPos, 3> a = am - half_width_a.casted<TPos>();
    FixedArray<TPos, 3> b = bm - half_width_b.casted<TPos>();
    FixedArray<TPos, 3> c = cm - half_width_c.casted<TPos>();
    if ((dot0d(bm - am, B - A) <= 0) ||
        (dot0d(cm - bm, C - B) <= 0) ||
        (dot0d(am - cm, A - C) <= 0) ||
        (dot0d(bm - am, b - a) <= 0) ||
        (dot0d(cm - bm, c - b) <= 0) ||
        (dot0d(am - cm, a - c) <= 0))
    {
        throw TriangleException<TPos>{am, bm, cm, "convex_decomposition_terrain: consistency-check failed"};
    }
    UUVector<FixedArray<TPos, 3, 3>> result;
    result.reserve(
        2 +
        ab_is_contour_edge * 2 +
        bc_is_contour_edge * 2 +
        ca_is_contour_edge * 2);

    result.emplace_back(a, b, c);
    if (ab_is_contour_edge) {
        result.emplace_back(a, A, b);
        result.emplace_back(A, B, b);
    }
    if (bc_is_contour_edge) {
        result.emplace_back(b, B, c);
        result.emplace_back(B, C, c);
    }
    if (ca_is_contour_edge) {
        result.emplace_back(c, C, a);
        result.emplace_back(C, A, a);
    }
    result.emplace_back(B, A, C);
    return result;
}

template <class TPos>
std::vector<std::shared_ptr<ColoredVertexArray<TPos>>> VA::create_barrier_triangle_hitboxes(
    const ColoredVertexArray<TPos>& cva,
    float half_width,
    PhysicsMaterial destination_physics_material)
{
    if (!any(cva.morphology.physics_material & PhysicsMaterial::ATTR_TWO_SIDED)) {
        throw std::runtime_error("Terrain not marked as two-sided");
    }
    if (!any(cva.morphology.physics_material & PhysicsMaterial::ATTR_COLLIDE)) {
        throw std::runtime_error("Terrain to be decomposed is not collidable");
    }
    if (!any(destination_physics_material & PhysicsMaterial::ATTR_CONCAVE)) {
        throw std::runtime_error("Destination mesh is not tagged as concave");
    }
    std::list<const FixedArray<ColoredVertex<TPos>, 3>*> triangle_ptrs;
    for (const auto& t : cva.triangles) {
        triangle_ptrs.push_back(&t);
    }
    auto contour_edges = find_contour_edges(triangle_ptrs);

    VertexNormals<TPos, float> vertex_normals;
    vertex_normals.add_triangles(cva.triangles.begin(), cva.triangles.end());
    vertex_normals.compute_vertex_normals(NormalVectorErrorBehavior::THROW);

    std::vector<std::shared_ptr<ColoredVertexArray<TPos>>> result;
    result.reserve(2);
    result.push_back(
        std::make_shared<ColoredVertexArray<TPos>>(
            cva.name + "_visual",
            cva.material,
            cva.morphology - PhysicsMaterial::ATTR_COLLIDE,
            cva.modifier_backlog,
            std::vector{cva.quads},
            std::vector{cva.triangles},
            std::vector{cva.lines},
            std::vector{cva.triangle_bone_weights},
            std::vector{cva.continuous_triangle_texture_layers},
            std::vector{cva.discrete_triangle_texture_layers},
            std::vector{cva.uv1},
            std::vector{cva.cweight},
            std::vector{cva.alpha},
            std::vector{cva.interiormap_uvmaps}));
    UUVector<FixedArray<ColoredVertex<TPos>, 3>> decomposition;
    decomposition.reserve(2 * cva.triangles.size() + 2 * contour_edges.size());
    for (const auto& tri : cva.triangles) {
        auto hitbox = barrier_triangle_hitbox(
            funpack(tri(0).position),
            funpack(tri(1).position),
            funpack(tri(2).position),
            vertex_normals.get_normal(tri(0).position) * (-half_width),
            vertex_normals.get_normal(tri(1).position) * (-half_width),
            vertex_normals.get_normal(tri(2).position) * (-half_width),
            contour_edges.contains({make_orderable(tri(0).position), make_orderable(tri(1).position)}),
            contour_edges.contains({make_orderable(tri(1).position), make_orderable(tri(2).position)}),
            contour_edges.contains({make_orderable(tri(2).position), make_orderable(tri(0).position)}));
        for (const auto& s : hitbox) {
            const auto zeros2 = fixed_zeros<float, 2>();
            if (decomposition.capacity() == decomposition.size()) {
                throw std::runtime_error("create_barrier_triangle_hitboxes internal error (0)");
            }
            auto n = triangle_normal(s).template casted<float>();
            decomposition.emplace_back(
                ColoredVertex<TPos>{s[0].template casted<TPos>(), Colors::PURPLE, zeros2, n},
                ColoredVertex<TPos>{s[1].template casted<TPos>(), Colors::PURPLE, zeros2, n},
                ColoredVertex<TPos>{s[2].template casted<TPos>(), Colors::PURPLE, zeros2, n});
        }
    }
    if (decomposition.capacity() != decomposition.size()) {
        throw std::runtime_error("create_barrier_triangle_hitboxes internal error (1)");
    }
    auto removed_attributes =
        PhysicsMaterial::ATTR_VISIBLE |
        PhysicsMaterial::ATTR_COLLIDE |
        PhysicsMaterial::ATTR_TWO_SIDED |
        PhysicsMaterial::ATTR_CONVEX |
        PhysicsMaterial::ATTR_CONCAVE;
    result.push_back(
        std::make_shared<ColoredVertexArray<TPos>>(
            cva.name + "_blk",
            Material{
                .aggregate_mode = AggregateMode::ONCE
            },
            (cva.morphology - removed_attributes) + destination_physics_material,
            cva.modifier_backlog,
            UUVector<FixedArray<ColoredVertex<TPos>, 4>>{},
            std::move(decomposition),
            UUVector<FixedArray<ColoredVertex<TPos>, 2>>{},
            UUVector<FixedArray<std::vector<BoneWeight>, 3>>{},
            UUVector<FixedArray<float, 3>>{},
            UUVector<FixedArray<uint8_t, 3>>{},
            std::vector<UUVector<FixedArray<float, 3, 2>>>{},
            std::vector<UUVector<FixedArray<float, 3>>>{},
            UUVector<FixedArray<float, 3>>{},
            UUVector<FixedArray<float, 4>>{}));
    return result;
}

template
std::vector<std::shared_ptr<ColoredVertexArray<CompressedScenePos>>> VA::create_barrier_triangle_hitboxes(
    const ColoredVertexArray<CompressedScenePos>& cva,
    float half_width,
    PhysicsMaterial destination_physics_material);
