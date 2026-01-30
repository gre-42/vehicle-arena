// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Time/Fps/Dependent_Sleeper.hpp>
#include <VehicleArena/Time/Fps/Frame_Time.hpp>
#include <VehicleArena/Time/Fps/Measure_Fps.hpp>
#include <VehicleArena/Time/Fps/Realtime_Sleeper.hpp>
#include <VehicleArena/Time/Fps/Set_Fps.hpp>
#include <VehicleArena/Time/Fps/Sleeper_Sequence.hpp>

namespace VA {

struct RealtimeDependentFps {
    explicit RealtimeDependentFps(
        std::string prefix,
        float dt,
        std::chrono::steady_clock::duration delay,
        float max_residual_time,
        bool control_fps,
        bool print_residual_time,
        float alpha0,
        float alpha1,
        unsigned int print_interval);
    RealtimeSleeper rts;
    DependentSleeper ds;
    MeasureFps mf;
    FrameTime ft;
    SleeperSequence sls;
    SetFps set_fps;
};

}
