// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Threads/Fast_Mutex.hpp>
#include <chrono>
#include <cstdint>
#include <mutex>

namespace VA {

class SafeAtomicSharedMutex {
    SafeAtomicSharedMutex(const SafeAtomicSharedMutex&) = delete;
    SafeAtomicSharedMutex& operator = (const SafeAtomicSharedMutex&) = delete;
public:
    inline SafeAtomicSharedMutex()
        : locked_{ false }
        , nlocked_shared_{ 0 }
    {}
    inline ~SafeAtomicSharedMutex() = default;
    inline void lock() {
        while (!try_lock());
    }
    template <class Rep, class Period>
    bool try_lock_for(const std::chrono::duration<Rep, Period>& timeout_duration)
    {
        auto start = std::chrono::steady_clock::now();
        while (!try_lock()) {
            auto now = std::chrono::steady_clock::now();
            if (now - start >= timeout_duration) {
                return false;
            }
        }
        return true;
    }
    inline bool try_lock() {
        if (!mutex_.try_lock()) {
            return false;
        }
        if (locked_ || (nlocked_shared_ != 0)) {
            mutex_.unlock();
            return false;
        }
        locked_ = true;
        mutex_.unlock();
        return true;
    }
    inline void unlock() {
        std::scoped_lock lock{ mutex_ };
        locked_ = false;
    }
    void lock_shared() {
        while (!try_lock_shared());
    }
    bool try_lock_shared() {
        std::scoped_lock lock{ mutex_ };
        if (locked_) {
            return false;
        }
        ++nlocked_shared_;
        return true;
    }
    void unlock_shared() {
        std::scoped_lock lock{ mutex_ };
        --nlocked_shared_;
    }
private:
    bool locked_;
    uint32_t nlocked_shared_;
    FastMutex mutex_;
};

}
