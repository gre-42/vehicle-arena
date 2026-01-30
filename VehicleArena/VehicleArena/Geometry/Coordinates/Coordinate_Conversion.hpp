// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <cstddef>

namespace VA {

template <typename TData, size_t... tshape>
class FixedArray;
template <class TDir, class TPos, size_t n>
class TransformationMatrix;

TransformationMatrix<float, float, 2> intrinsic_matrix_from_dimensions(
    float focal_length,
    const FixedArray<float, 2>& sensor_size,
    const FixedArray<size_t, 2>& picture_shape);

TransformationMatrix<float, float, 2> rotated_intrinsic_matrix(
    const TransformationMatrix<float, float, 2>& intrinsic_matrix,
    const FixedArray<size_t, 2>& sensor_size,
    int num_rotations);

/**
 * From:
 *   - https://fruty.io/2019/08/29/augmented-reality-with-opencv-and-opengl-the-tricky-projection-matrix/
 *   - https://strawlab.org/2011/11/05/augmented-reality-with-OpenGL/
 */
FixedArray<float, 4, 4> cv_to_opengl_hz_intrinsic_matrix(
    const TransformationMatrix<float, float, 2>& intrinsic_matrix,
    float width,
    float height,
    float z_near,
    float z_far);

FixedArray<float, 3> cv_to_opengl_coordinates(
    const FixedArray<float, 3>& p);

TransformationMatrix<float, float, 3> cv_to_opengl_extrinsic_matrix(
    const TransformationMatrix<float, float, 3>& extrinsic_matrix);

TransformationMatrix<float, float, 3> opengl_to_cv_extrinsic_matrix(
    const TransformationMatrix<float, float, 3>& extrinsic_matrix);

TransformationMatrix<float, float, 3> cv_to_opengl_matrix();

TransformationMatrix<float, float, 3> opengl_to_cv_matrix();

}
