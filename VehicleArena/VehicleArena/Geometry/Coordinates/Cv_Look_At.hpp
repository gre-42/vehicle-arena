// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Geometry/Coordinates/Coordinate_Conversion.hpp>
#include <VehicleArena/Geometry/Coordinates/Gl_Look_At.hpp>
#include <VehicleArena/Math/Transformation/Transformation_Matrix.hpp>

namespace VA {

template <class TData>
TransformationMatrix<TData, TData, 3> cv_lookat_relative(
    const FixedArray<TData, 3>& camera_pos,
    const FixedArray<TData, 3>& dz,
    const FixedArray<TData, 3>& dy0 = { 0.f, 1.f, 0.f })
{
    auto R = gl_lookat_relative(
        cv_to_opengl_coordinates(dz),
        cv_to_opengl_coordinates(dy0));
    if (!R.has_value()) {
        THROW_OR_ABORT("Could not compute lookat-matrix");
    }
    return opengl_to_cv_extrinsic_matrix(
        TransformationMatrix<float, float, 3>{
            *R,
            cv_to_opengl_coordinates(camera_pos) });
}

}
