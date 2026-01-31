// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Perspective_Camera.hpp"
#include <VehicleArena/Array/Fixed_Array.hpp>
#include <VehicleArena/Geometry/linmath.hpp>
#include <mutex>

using namespace VA;

PerspectiveCamera::PerspectiveCamera(
    const PerspectiveCameraConfig& cfg,
    Postprocessing postprocessing)
    : cfg_{ cfg }
    , postprocessing_{ postprocessing }
{}

PerspectiveCamera::~PerspectiveCamera() = default;

std::unique_ptr<Camera> PerspectiveCamera::copy() const {
    std::shared_lock lock{mutex_};
    return std::make_unique<PerspectiveCamera>(cfg_, postprocessing_);
}

void PerspectiveCamera::set_y_fov(float y_fov) {
    std::scoped_lock lock{mutex_};
    cfg_.y_fov = y_fov;
}

void PerspectiveCamera::set_aspect_ratio(float aspect_ratio) {
    std::scoped_lock lock{mutex_};
    cfg_.aspect_ratio = aspect_ratio;
}

void PerspectiveCamera::set_near_plane(float near_plane) {
    std::scoped_lock lock{mutex_};
    cfg_.near_plane = near_plane;
}

void PerspectiveCamera::set_far_plane(float far_plane) {
    std::scoped_lock lock{mutex_};
    cfg_.far_plane = far_plane;
}

float PerspectiveCamera::get_near_plane() const {
    std::shared_lock lock{mutex_};
    return cfg_.near_plane;
}

float PerspectiveCamera::get_far_plane() const {
    std::shared_lock lock{mutex_};
    return cfg_.far_plane;
}

void PerspectiveCamera::set_requires_postprocessing(bool value) {
    std::scoped_lock lock{mutex_};
    postprocessing_ = (Postprocessing)value;
}

bool PerspectiveCamera::get_requires_postprocessing() const {
    std::shared_lock lock{mutex_};
    return (bool)postprocessing_;
}

FixedArray<float, 4, 4> PerspectiveCamera::projection_matrix() const {
    std::shared_lock lock{mutex_};
    mat4x4 p;
    mat4x4_perspective(p, cfg_.y_fov, cfg_.aspect_ratio, cfg_.near_plane, cfg_.far_plane);
    //mat4x4_frustum(p, -ratio / 10, ratio / 10, -1.f / 10, 1.f / 10, 2.f, 10.f);
    static_assert(sizeof(p) == sizeof(FixedArray<float, 4, 4>));
    return reinterpret_cast<FixedArray<float, 4, 4>*>(&p)->T();
}

FixedArray<float, 2> PerspectiveCamera::dpi(const FixedArray<float, 2>& texture_size) const {
    return cfg_.dpi(texture_size);
}
