// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Array/Fixed_Array.hpp>

namespace VA {

class NormalizedPoints {
public:
    explicit NormalizedPoints(bool preserve_aspect_ratio, bool centered);
    void add_point(const FixedArray<float, 2>& p);
    void add_points_quantile(const Array<FixedArray<float, 2>>& p, float q);
    FixedArray<float, 3, 3> normalization_matrix() const;
private:
    float min_x_ = std::numeric_limits<float>::infinity();
    float max_x_ = -std::numeric_limits<float>::infinity();
    float min_y_ = std::numeric_limits<float>::infinity();
    float max_y_ = -std::numeric_limits<float>::infinity();
    bool preserve_aspect_ratio_;
    bool centered_;
};

}
