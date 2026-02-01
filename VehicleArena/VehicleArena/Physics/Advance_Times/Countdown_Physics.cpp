// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Countdown_Physics.hpp"
#include <VehicleArena/Memory/Float_To_Integral.hpp>
#include <VehicleArena/Physics/Units.hpp>
#include <mutex>

using namespace VA;

CountdownPhysics::CountdownPhysics()
    : elapsed_{ NAN }
    , duration_{ NAN }
{}

CountdownPhysics::~CountdownPhysics() {
    on_destroy.clear();
}

void CountdownPhysics::set(float elapsed, float duration) {
    std::scoped_lock lock{ mutex_ };
    elapsed_ = elapsed;
    duration_ = duration;
}

float CountdownPhysics::elapsed() const {
    std::scoped_lock lock{ mutex_ };
    return elapsed_;
}

float CountdownPhysics::duration() const {
    std::scoped_lock lock{ mutex_ };
    return duration_;
}

void CountdownPhysics::reset(float duration) {
    std::scoped_lock lock{ mutex_ };
    duration_ = duration;
    elapsed_ = 0.f;
}

uint32_t CountdownPhysics::seconds_remaining() const {
    std::scoped_lock lock{ mutex_ };
    if (std::isnan(duration_)) {
        return 0;
    }
    return float_to_integral<uint32_t>(std::ceil((duration_ - elapsed_) / seconds));
}

bool CountdownPhysics::counting() const {
    std::scoped_lock lock{ mutex_ };
    if (std::isnan(duration_)) {
        return false;
    }
    return elapsed_ < duration_;
}

bool CountdownPhysics::finished() const {
    return !counting();
}

void CountdownPhysics::advance_time(float dt, const StaticWorld& world) {
    std::scoped_lock lock{ mutex_ };
    if (!std::isnan(duration_)) {
        elapsed_ = std::min(duration_, elapsed_ + dt);
    }
}
