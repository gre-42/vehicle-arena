// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Initialization/Default_Uninitialized_Vector.hpp>
#include <cstddef>
#include <cstdint>
#include <list>
#include <memory>

namespace VA {

template <class TPos>
class ColoredVertexArray;
enum class PhysicsMaterial: uint32_t;
template <typename TData, size_t... tshape>
class FixedArray;

template <class TPos>
UUVector<FixedArray<TPos, 3, 3>> barrier_triangle_hitbox(
    const FixedArray<TPos, 3>& am,
    const FixedArray<TPos, 3>& bm,
    const FixedArray<TPos, 3>& cm,
    const FixedArray<float, 3>& half_width_a,
    const FixedArray<float, 3>& half_width_b,
    const FixedArray<float, 3>& half_width_c,
    bool ab_is_contour_edge,
    bool bc_is_contour_edge,
    bool ca_is_contour_edge);

template <class TPos>
std::vector<std::shared_ptr<ColoredVertexArray<TPos>>> create_barrier_triangle_hitboxes(
    const ColoredVertexArray<TPos>& cva,
    float half_width,
    PhysicsMaterial destination_physics_material);

}
