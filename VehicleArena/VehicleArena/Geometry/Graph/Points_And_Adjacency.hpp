// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Array/Fixed_Array.hpp>
#include <VehicleArena/Array/Sparse_Array.hpp>
#include <VehicleArena/Initialization/Default_Uninitialized_Vector.hpp>

namespace VA {

enum class SubdivisionType {
    SYMMETRIC,
    ASYMMETRIC,
    MAKE_SYMMETRIC
};

template <class TDir, class TPos, size_t n>
class TransformationMatrix;
template <class TSize>
class Svg;

template <class TPoint>
struct PointsAndAdjacency {
    using TData = typename TPoint::value_type;
    static const size_t tlength = TPoint::length();
    UVector<TPoint> points;
    SparseArrayCcs<TData> adjacency;

    PointsAndAdjacency() = default;
    explicit PointsAndAdjacency(size_t npoints);
    ~PointsAndAdjacency() = default;

    template <class Archive>
    void serialize(Archive& archive) {
        archive(points);
        archive(adjacency);
    }
    void transform(const TransformationMatrix<float, TData, tlength>& m);
    void update_adjacency();
    void update_adjacency_diagonal();
    template <class TCalculateIntermediatePoints>
    void subdivide(
        const TCalculateIntermediatePoints& calculate_intermediate_points,
        SubdivisionType subdivision_type);
    PointsAndAdjacency concatenated(const PointsAndAdjacency& other) const;
    void insert(const PointsAndAdjacency& other);
    template <class TCombinePoints>
    PointsAndAdjacency merged_neighbors(
        const TData& merge_radius,
        const TData& error_radius,
        const TCombinePoints& combine_points) const;
    template <class TCombinePoints>
    void merge_neighbors(
        const TData& merge_radius,
        const TData& error_radius,
        const TCombinePoints& combine_points);
    template <class TSize>
    void plot(Svg<TSize>& svg, float line_width = 1.5) const;
    void plot(const std::string& filename, float width, float height, float line_width = 1.5) const;
};

}
