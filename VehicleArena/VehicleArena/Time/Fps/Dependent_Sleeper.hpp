// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Threads/Safe_Atomic_Shared_Mutex.hpp>
#include <VehicleArena/Time/Fps/ISleeper.hpp>
#include <unordered_set>

namespace VA {

class SetFps;

class DependentSleeper: public ISleeper {
    DependentSleeper(const DependentSleeper&) = delete;
    DependentSleeper& operator = (const DependentSleeper&) = delete;
public:
    explicit DependentSleeper();
    ~DependentSleeper();
    virtual void tick() override;
    virtual void reset() override;
    virtual bool is_up_to_date() const override;

    void register_busy_state_provider(const SetFps& busy_state_provider);
    void unregister_busy_state_provider(const SetFps& busy_state_provider);
private:
    std::unordered_set<const SetFps*> busy_state_providers_;
    mutable SafeAtomicSharedMutex mutex_;
};

class BusyStateProviderGuard {
public:
    inline BusyStateProviderGuard(
        DependentSleeper& sleeper,
        const SetFps& busy_state_provider)
    : sleeper_{sleeper},
      busy_state_provider_{busy_state_provider}
    {
        sleeper_.register_busy_state_provider(busy_state_provider);
    }
    inline ~BusyStateProviderGuard() {
        sleeper_.unregister_busy_state_provider(busy_state_provider_);
    }
private:
    DependentSleeper& sleeper_;
    const SetFps& busy_state_provider_;
};

}
