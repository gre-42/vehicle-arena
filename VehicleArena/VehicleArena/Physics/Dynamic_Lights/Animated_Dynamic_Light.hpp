// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Array/Fixed_Array.hpp>
#include <VehicleArena/Math/Interp.hpp>
#include <VehicleArena/Math/Transformation/Position_Series.hpp>
#include <VehicleArena/Scene_Graph/Interfaces/IDynamic_Light.hpp>
#include <VehicleArena/Scene_Graph/Interpolation.hpp>
#include <chrono>
#include <functional>

namespace VA {

class DynamicLights;

struct AnimatedDynamicLightConfiguration {
    UUInterp<float, FixedArray<float, 3>> time_to_color = { {}, {} };
    Interp<ScenePos, float> squared_distance_to_intensity = { {}, {} };
};

class AnimatedDynamicLight: public IDynamicLight {
public:
    AnimatedDynamicLight(
        std::function<FixedArray<ScenePos, 3>()> get_position,
        std::chrono::steady_clock::time_point time,
        const AnimatedDynamicLightConfiguration& config,
        DynamicLights& container);
    virtual ~AnimatedDynamicLight() override;
    virtual void append_time(std::chrono::steady_clock::time_point time) override;
    virtual void set_time(std::chrono::steady_clock::time_point time) override;
    virtual FixedArray<float, 3> get_color(const FixedArray<ScenePos, 3>& target_position) const override;
    virtual bool animation_completed(std::chrono::steady_clock::time_point time) const override;

private:
    float elapsed(std::chrono::steady_clock::time_point time) const;

    std::chrono::steady_clock::time_point creation_time_;
    std::function<FixedArray<ScenePos, 3>()> get_position_;
    PositionSeries<ScenePos, 3, NINTERPOLATED> position_history_;
    const AnimatedDynamicLightConfiguration& config_;
    DynamicLights& container_;
    FixedArray<ScenePos, 3> position_;
    FixedArray<float, 3> color_;
};

}
