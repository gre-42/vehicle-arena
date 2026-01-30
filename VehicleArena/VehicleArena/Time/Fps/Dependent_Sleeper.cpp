// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Dependent_Sleeper.hpp"
#include <VehicleArena/Os/Os.hpp>
#include <VehicleArena/Time/Fps/Set_Fps.hpp>
#include <mutex>
#include <shared_mutex>
#include <stdexcept>
#include <thread>

using namespace VA;

DependentSleeper::DependentSleeper() = default;

DependentSleeper::~DependentSleeper() = default;

void DependentSleeper::tick() {
    while (!is_up_to_date());
}

void DependentSleeper::reset() {
    // Do nothing
}

bool DependentSleeper::is_up_to_date() const {
    std::shared_lock lock{ mutex_ };
    for (const auto& b : busy_state_providers_) {
        if (!b->is_up_to_date()) {
            return false;
        }
    }
    return true;
}

void DependentSleeper::register_busy_state_provider(const SetFps& busy_state_provider) {
    std::scoped_lock lock{mutex_};
    if (!busy_state_providers_.insert(&busy_state_provider).second) {
        throw std::runtime_error("Busy state provider already registered");
    }
}

void DependentSleeper::unregister_busy_state_provider(const SetFps& busy_state_provider) {
    std::scoped_lock lock{mutex_};
    if (busy_state_providers_.erase(&busy_state_provider) != 1) {
        verbose_abort("Busy state provider not registered");
    }
}
