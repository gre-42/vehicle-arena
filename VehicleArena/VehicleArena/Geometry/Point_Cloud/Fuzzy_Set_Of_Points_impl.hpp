// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Fuzzy_Set_Of_Points.hpp"
#include <VehicleArena/Geometry/Exceptions/Point_Exception.hpp>

namespace VA {

template <class TData, size_t tndim>
FuzzySetOfPoints<TData, tndim>::FuzzySetOfPoints(
    const TData& merge_radius,
    const TData& error_radius)
    : bvh_{ fixed_full<TData, tndim>(merge_radius), 17 }
    , bvh_size_{ 0 }
    , merge_radius_{ merge_radius }
    , error_radius_{ error_radius }
{}

template <class TData, size_t tndim>
bool FuzzySetOfPoints<TData, tndim>::insert(const FixedArray<TData, tndim>& p, size_t& index) {
    const std::pair<FixedArray<TData, tndim>, size_t>* neighbor;
    auto min_dist2 = bvh_.min_distance(
        p,
        merge_radius_,
        [&p](const auto& a){return sum(squared(a.first - p));},
        &neighbor);
    if (min_dist2.has_value() && (*min_dist2 < squared(merge_radius_))) {
        index = neighbor->second;
        return false;
    } else if (min_dist2.has_value() && (*min_dist2 < squared(error_radius_))) {
        throw PointException{ p, "Detected problematic fuzzy point distance" };
    } else {
        index = bvh_size_++;
        bvh_.insert(
            AxisAlignedBoundingBox<TData, tndim>::from_point(p),
            std::pair<FixedArray<TData, tndim>, size_t>{ p, index });
        return true;
    }
}

template <class TData, size_t tndim>
void FuzzySetOfPoints<TData, tndim>::optimize_search_time(std::ostream& ostr) const {
    bvh_.optimize_search_time(BvhDataRadiusType::ZERO, ostr);
}

}
