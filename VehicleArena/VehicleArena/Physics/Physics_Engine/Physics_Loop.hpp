// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Threads/J_Thread.hpp>
#include <atomic>
#include <functional>
#include <string>
#include <thread>

namespace VA {

enum class ThreadAffinity;
class PhysicsIteration;
class SetFps;

class PhysicsLoop {
public:
    PhysicsLoop(
        std::string thread_name,
        ThreadAffinity thread_affinity,
        PhysicsIteration& physics_iteration,
        std::function<bool()> level_loading,
        SetFps& set_fps,
        size_t nframes = SIZE_MAX,
        const std::function<std::function<void()>(std::function<void()>)>& run_in_background = [](std::function<void()> f){return f;});
    ~PhysicsLoop();
    void stop_and_join();
    //! Useful if nframes != SIZE_MAX
    void join();
private:
    SetFps& set_fps_;
    PhysicsIteration& physics_iteration_;
    JThread physics_thread_;
};

}
