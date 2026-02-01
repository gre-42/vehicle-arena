// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Misc/Ignore_Copy.hpp>
#include <VehicleArena/Threads/Safe_Atomic_Shared_Mutex.hpp>
#include <string>

namespace VA {

enum class AnimationWrapMode {
    PERIODIC,
    APERIODIC
};

struct AnimationFrame {
    float begin;
    float end;
    float time;
    void advance_time(float dt, AnimationWrapMode wrap_mode);
    bool is_nan() const;
};

class PeriodicAnimationFrame {
public:
    PeriodicAnimationFrame(const AnimationFrame& frame)
        : frame_{ frame }
    {}
    float time() const;
    void advance_time(float dt);
private:
    AnimationFrame frame_;
    mutable IgnoreCopy<SafeAtomicSharedMutex> mutex_;
};

class AperiodicAnimationFrame {
public:
    AperiodicAnimationFrame(const AnimationFrame& frame)
        : frame_{ frame }
    {}
    float time() const;
    void advance_time(float dt);
    bool active() const;
    bool ran_to_completion() const;
    float duration() const;
    float elapsed() const;
    bool is_nan() const;
private:
    AnimationFrame frame_;
    mutable IgnoreCopy<SafeAtomicSharedMutex> mutex_;
};

}
