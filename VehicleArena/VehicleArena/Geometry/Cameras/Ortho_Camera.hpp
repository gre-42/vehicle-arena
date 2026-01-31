// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Geometry/Cameras/Camera.hpp>
#include <VehicleArena/Geometry/Cameras/Ortho_Camera_Config.hpp>
#include <VehicleArena/Threads/Recursive_Shared_Mutex.hpp>

namespace VA {

class OrthoCamera final: public Camera {
public:
    enum class Postprocessing {
        DISABLED = 0,
        ENABLED = 1
    };
    explicit OrthoCamera(
        const OrthoCameraConfig& cfg,
        Postprocessing postprocessing);
    virtual ~OrthoCamera() override;
    virtual std::unique_ptr<Camera> copy() const override;
    virtual FixedArray<float, 4, 4> projection_matrix() const override;
    virtual FixedArray<float, 2> dpi(const FixedArray<float, 2>& texture_size) const override;
    virtual float get_near_plane() const override;
    virtual float get_far_plane() const override;
    virtual void set_requires_postprocessing(bool value) override;
    virtual bool get_requires_postprocessing() const override;

    float get_left_plane() const;
    float get_right_plane() const;
    float get_bottom_plane() const;
    float get_top_plane() const;

    void set_near_plane(float near_plane);
    void set_far_plane(float far_plane);
    void set_left_plane(float left_plane);
    void set_right_plane(float right_plane);
    void set_bottom_plane(float bottom_plane);
    void set_top_plane(float top_plane);

    FixedArray<float, 2> grid(const FixedArray<float, 2>& texture_size) const;
private:
    OrthoCameraConfig cfg_;
    Postprocessing postprocessing_;
    mutable SafeAtomicRecursiveSharedMutex mutex_;
};

}
