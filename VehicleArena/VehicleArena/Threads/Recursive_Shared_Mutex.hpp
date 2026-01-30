// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Threads/Safe_Atomic_Shared_Mutex.hpp>
#include <shared_mutex>
#include <thread>

namespace VA {

template <class TMutex>
class GenericRecursiveSharedMutex {
public:
    GenericRecursiveSharedMutex()
        : owner_{ std::thread::id() }
        , count_{ 0 }
    {}
    ~GenericRecursiveSharedMutex() = default;
    void lock() {
        if (!is_owner()) {
            mutex_.lock();
            owner_ = std::this_thread::get_id();
        }
        ++count_;
    }
    bool try_lock() {
        if (!is_owner()) {
            if (!mutex_.try_lock()) {
                return false;
            }
            owner_ = std::this_thread::get_id();
        }
        ++count_;
        return true;
    }
    void unlock() {
        --count_;
        if (count_ == 0) {
            owner_ = std::thread::id();
            mutex_.unlock();
        }
    }
    void lock_shared() {
        if (!is_owner()) {
            mutex_.lock_shared();
        }
    }
    void unlock_shared() {
        if (!is_owner()) {
            mutex_.unlock_shared();
        }
    }
    bool is_owner() const {
        return (owner_ == std::this_thread::get_id());
    }
private:
    TMutex mutex_;
    std::atomic<std::thread::id> owner_;
    uint32_t count_;
};

using RecursiveSharedMutex = GenericRecursiveSharedMutex<std::shared_mutex>;
using SafeAtomicRecursiveSharedMutex = GenericRecursiveSharedMutex<SafeAtomicSharedMutex>;

}
