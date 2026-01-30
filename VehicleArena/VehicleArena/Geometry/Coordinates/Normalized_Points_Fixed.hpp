// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Array/Fixed_Array.hpp>

namespace VA {

enum class ScaleMode {
    NONE,
    PRESERVE_ASPECT_RATIO,
    DIAGONAL
};

enum class OffsetMode {
    CENTERED,
    MINIMUM
};

template <class TDir, class TPos, size_t n>
class TransformationMatrix;

template <class TData>
class NormalizedPointsFixed {
public:
    NormalizedPointsFixed(ScaleMode scale_mode, OffsetMode offset_mode);
    void add_point(const FixedArray<TData, 2>& p);
    void set_min(const FixedArray<TData, 2>& p);
    void set_max(const FixedArray<TData, 2>& p);
    TransformationMatrix<TData, TData, 2> normalization_matrix() const;
    NormalizedPointsFixed chained(ScaleMode scale_mode, OffsetMode offset_mode) const;
    const FixedArray<TData, 2>& min() const;
    const FixedArray<TData, 2>& max() const;
private:
    FixedArray<TData, 2> min_{TData(INFINITY), TData(INFINITY)};
    FixedArray<TData, 2> max_{-TData(INFINITY), -TData(INFINITY)};
    ScaleMode scale_mode_;
    OffsetMode offset_mode_;
};

}
