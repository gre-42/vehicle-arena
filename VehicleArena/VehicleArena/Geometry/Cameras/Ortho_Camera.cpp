// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Ortho_Camera.hpp"
#include <VehicleArena/Array/Fixed_Array.hpp>
#include <VehicleArena/Geometry/linmath.hpp>
#include <mutex>

using namespace VA;

OrthoCamera::OrthoCamera(
    const OrthoCameraConfig& cfg,
    Postprocessing postprocessing)
    : cfg_{ cfg }
    , postprocessing_{ postprocessing }
{}

OrthoCamera::~OrthoCamera() = default;

std::unique_ptr<Camera> OrthoCamera::copy() const {
    std::shared_lock lock{mutex_};
    return std::make_unique<OrthoCamera>(cfg_, postprocessing_);
}

void OrthoCamera::set_near_plane(float near_plane) {
    std::scoped_lock lock{mutex_};
    cfg_.near_plane = near_plane;
}

void OrthoCamera::set_far_plane(float far_plane) {
    std::scoped_lock lock{mutex_};
    cfg_.far_plane = far_plane;
}

void OrthoCamera::set_left_plane(float left_plane) {
    std::scoped_lock lock{mutex_};
    cfg_.left_plane = left_plane;
}

void OrthoCamera::set_right_plane(float right_plane) {
    std::scoped_lock lock{mutex_};
    cfg_.right_plane = right_plane;
}

void OrthoCamera::set_bottom_plane(float bottom_plane) {
    std::scoped_lock lock{mutex_};
    cfg_.bottom_plane = bottom_plane;
}

void OrthoCamera::set_top_plane(float top_plane) {
    std::scoped_lock lock{mutex_};
    cfg_.top_plane = top_plane;
}

float OrthoCamera::get_near_plane() const {
    std::shared_lock lock{mutex_};
    return cfg_.near_plane;
}

float OrthoCamera::get_far_plane() const {
    std::shared_lock lock{mutex_};
    return cfg_.far_plane;
}

float OrthoCamera::get_left_plane() const {
    std::shared_lock lock{mutex_};
    return cfg_.left_plane;
}

float OrthoCamera::get_right_plane() const {
    std::shared_lock lock{mutex_};
    return cfg_.right_plane;
}

float OrthoCamera::get_bottom_plane() const {
    std::shared_lock lock{mutex_};
    return cfg_.bottom_plane;
}

float OrthoCamera::get_top_plane() const {
    std::shared_lock lock{mutex_};
    return cfg_.top_plane;
}

void OrthoCamera::set_requires_postprocessing(bool value) {
    std::scoped_lock lock{mutex_};
    postprocessing_ = (Postprocessing)value;
}

bool OrthoCamera::get_requires_postprocessing() const {
    std::shared_lock lock{mutex_};
    return (bool)postprocessing_;
}

FixedArray<float, 4, 4> OrthoCamera::projection_matrix() const {
    std::shared_lock lock{mutex_};
    mat4x4 p;
    mat4x4_ortho(p, cfg_.left_plane, cfg_.right_plane, cfg_.bottom_plane, cfg_.top_plane, cfg_.near_plane, cfg_.far_plane);
    //mat4x4_frustum(p, -ratio / 10, ratio / 10, -1.f / 10, 1.f / 10, 2.f, 10.f);
    static_assert(sizeof(p) == sizeof(FixedArray<float, 4, 4>));
    return reinterpret_cast<FixedArray<float, 4, 4>*>(&p)->T();
}

FixedArray<float, 2> OrthoCamera::dpi(const FixedArray<float, 2>& texture_size) const {
    return {
        texture_size(0) / (get_right_plane() - get_left_plane()),
        texture_size(1) / (get_top_plane() - get_bottom_plane())};
}

FixedArray<float, 2> OrthoCamera::grid(const FixedArray<float, 2>& texture_size) const {
    return {
        (get_right_plane() - get_left_plane()) / texture_size(0),
        (get_top_plane() - get_bottom_plane()) / texture_size(1)};
}
