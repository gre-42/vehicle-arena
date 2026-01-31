// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Geometry/Primitives/Bvh.hpp>
#include <VehicleArena/Geometry/Primitives/Intersection_Grid.hpp>

namespace VA {

template <class TPosition, size_t tndim, class TPayload>
class GenericBvhGrid {
public:
    using TBvh = GenericBvh<TPosition, tndim, TPayload>;
    using Entry = IntersectionGridPointerEntry<const TBvh>;
    using Grid = IntersectionGrid<TPosition, tndim, IntersectionGridEntries<Entry, Entry>>;
    GenericBvhGrid(
        // BVH
        const FixedArray<TPosition, tndim>& max_size,
        size_t level,
        // Transition
        size_t grid_level,
        // Grid
        const FixedArray<size_t, tndim>& ncells,
        const FixedArray<TPosition, tndim>& dilation_radius)
        : root_bvh{ max_size, level }
        , grid_{ std::nullopt }
        , ncells_{ ncells }
        , dilation_radius_{ dilation_radius }
        , grid_level_{ grid_level }
    {}
    void clear() {
        grid_.reset();
        root_bvh.clear();
    }
    TBvh root_bvh;
    Grid& grid() const {
        if (!grid_.has_value()) {
            compute_grid();
        }
        return *grid_;
    }
private:
    void compute_grid() const {
        auto boundary = root_bvh.aabb();
        boundary.min -= dilation_radius_;
        boundary.max += dilation_radius_;
        grid_.emplace(boundary, ncells_, dilation_radius_);
        insert_into_grid(root_bvh, 0);
    }
    void insert_into_grid(const TBvh& b, size_t depth) const {
        if (depth == grid_level_) {
            grid_->insert(b.aabb(), IntersectionGridPointerEntry{&b}, recursive_v);
        } else {
            if (!b.data().empty()) {
                grid_->insert(b.data_aabb(), IntersectionGridPointerEntry{&b}, non_recursive_v);
            }
            for (const auto& [_, c] : b.children()) {
                insert_into_grid(c, depth + 1);
            }
        }
    }
    mutable std::optional<Grid> grid_;
    FixedArray<size_t, tndim> ncells_;
    FixedArray<TPosition, tndim> dilation_radius_;
    size_t grid_level_;
};

template <class TPosition, size_t tndim, class TPayload>
using BvhGrid = GenericBvhGrid<
    TPosition,
    tndim,
    PayloadContainer<std::list<AabbAndPayload<TPosition, tndim, TPayload>>>>;

template <class TPosition, class TCompressedPosition, class TPayload, class TCompressedPayload, size_t tndim>
using CompressedBvhGrid = GenericBvhGrid<
        TPosition,
        tndim,
        CompressedPayloadContainer<
                TPosition,
                tndim,
                std::list<AabbAndPayload<TCompressedPosition, tndim, TCompressedPayload>>,
                std::list<AabbAndPayload<TPosition, tndim, TPayload>>>>;

}
