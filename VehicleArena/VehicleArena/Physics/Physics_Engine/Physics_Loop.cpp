// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Physics_Loop.hpp"
#include <VehicleArena/Physics/Physics_Engine/Physics_Iteration.hpp>
#include <VehicleArena/Scene_Config/Physics_Engine_Config.hpp>
#include <VehicleArena/Threads/Termination_Manager.hpp>
#include <VehicleArena/Threads/Thread_Initializer.hpp>
#include <VehicleArena/Time/Fps/Lag_Finder.hpp>
#include <VehicleArena/Time/Fps/Set_Fps.hpp>
#include <VehicleArena/Time/Time_And_Pause.hpp>
#include <chrono>
#include <vector>

using namespace VA;

PhysicsLoop::PhysicsLoop(
    std::string thread_name,
    ThreadAffinity thread_affinity,
    PhysicsIteration& physics_iteration,
    std::function<bool()> level_loading,
    SetFps& set_fps,
    size_t nframes,
    const std::function<std::function<void()>(std::function<void()>)>& run_in_background)
    : set_fps_{set_fps}
    , physics_iteration_{physics_iteration}
    , physics_thread_{run_in_background(
        [this, tn=std::move(thread_name), thread_affinity, nframes, ll=std::move(level_loading)]()
        {
            try {
                ThreadInitializer ti{ tn, thread_affinity };
                size_t nframes2 = nframes;
                auto simulated_time = set_fps_.simulated_time();
                // PeriodicLagFinder lag_finder{ "Physics: ", std::chrono::milliseconds{ 100 }};
                while (!physics_thread_.get_stop_token().stop_requested()) {
                    auto loading = ll();
                    if (!set_fps_.paused() && !loading) {
                        // lag_finder.start();
                        // TimeGuard::initialize(5 * 60);
                        if (nframes2 != SIZE_MAX) {
                            if (nframes2-- == 0) {
                                break;
                            }
                        }
                        simulated_time = set_fps_.simulated_time();
                        physics_iteration_({simulated_time, PauseStatus::RUNNING});
                        // lerr() << rb0->get_new_absolute_model_matrix();
                        // TimeGuard tg2{"physics tick"};
                        set_fps_.tick(simulated_time);
                    } else {
                        physics_iteration_({simulated_time, loading ? PauseStatus::LOADING : PauseStatus::PAUSED});
                        if (!loading) {
                            set_fps_.tick(std::chrono::steady_clock::time_point());
                        }
                    }
                    // TimeGuard::print_groups(lraw());
                    // lag_finder.stop();
                }
            } catch (...) {
                add_unhandled_exception(std::current_exception());
            }
            })}
{}

PhysicsLoop::~PhysicsLoop() {
    stop_and_join();
}

void PhysicsLoop::stop_and_join() {
    physics_thread_.request_stop();
    set_fps_.request_stop();
    join();
}

void PhysicsLoop::join() {
    if (physics_thread_.joinable()) {
        physics_thread_.join();
    }
}
