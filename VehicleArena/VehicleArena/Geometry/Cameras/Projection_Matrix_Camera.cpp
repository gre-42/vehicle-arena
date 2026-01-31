// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Projection_Matrix_Camera.hpp"
#include <VehicleArena/Array/Fixed_Array.hpp>
#include <stdexcept>

using namespace VA;

ProjectionMatrixCamera::ProjectionMatrixCamera(const FixedArray<float, 4, 4>& projection_matrix)
    : projection_matrix_{ projection_matrix }
{}

ProjectionMatrixCamera::~ProjectionMatrixCamera() = default;

std::unique_ptr<Camera> ProjectionMatrixCamera::copy() const {
    return std::make_unique<ProjectionMatrixCamera>(projection_matrix_);
}

FixedArray<float, 4, 4> ProjectionMatrixCamera::projection_matrix() const {
    return projection_matrix_;
}

FixedArray<float, 2> ProjectionMatrixCamera::dpi(const FixedArray<float, 2>& texture_size) const {
    throw std::runtime_error("DPI computation not implemented for ProjectionMatrixCamera");
}
