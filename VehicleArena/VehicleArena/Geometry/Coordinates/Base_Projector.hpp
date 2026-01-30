// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Array/Array.hpp>
#include <VehicleArena/Geometry/Coordinates/Homogeneous.hpp>
#include <VehicleArena/Images/Coordinates_Fixed.hpp>


namespace VA {

class BaseProjector {

public:
    static inline FixedArray<float, 3, 3> nan_scale_matrix() {
        return fixed_nans<float, 3, 3>();
    }

    BaseProjector(
        size_t i0,
        size_t i1,
        size_t iz,
        const FixedArray<float, 3, 3>& scale_matrix = nan_scale_matrix())
    :i0_(i0),
     i1_(i1),
     iz_(iz),
     scale_matrix_(scale_matrix)
    {}

protected:
    FixedArray<size_t, 2> x2i(const FixedArray<float, 3>& x) {
        if (all(VA::isnan(scale_matrix_))) {
            return a2i(project(x));
        } else {
            return na2i(project(x));
        }
    }

    FixedArray<float, 2> x2fi(const FixedArray<float, 3>& x) {
        if (all(VA::isnan(scale_matrix_))) {
            return a2fi(project(x));
        } else {
            return na2fi(project(x));
        }
    }

    FixedArray<float, 2> project(const FixedArray<float, 3>& x) {
        FixedArray<float, 2> sliced{x(i0_), x(i1_)};
        if (all(VA::isnan(scale_matrix_))) {
            return sliced;
        } else {
            return dot1d(scale_matrix_, homogenized_3(sliced)).template row_range<0, 2>();
        }
    }

    float zcoord(const FixedArray<float, 3>& x) {
        return x(iz_);
    }

    size_t i0_;
    size_t i1_;
    size_t iz_;

protected:
    FixedArray<float, 3, 3> scale_matrix_;
};

}
