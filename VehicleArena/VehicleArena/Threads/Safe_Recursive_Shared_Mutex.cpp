// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Safe_Recursive_Shared_Mutex.hpp"
#include <VehicleArena/Os/Os.hpp>
#include <stdexcept>

using namespace VA;

SafeRecursiveSharedMutex::SafeRecursiveSharedMutex(size_t max_map_size)
    : max_map_size_{ max_map_size }
{}

SafeRecursiveSharedMutex::~SafeRecursiveSharedMutex() = default;

void SafeRecursiveSharedMutex::lock() {
    throw_if_mutex_upgrade();
    mutex_.lock();
}

void SafeRecursiveSharedMutex::unlock() {
    mutex_.unlock();
}

bool SafeRecursiveSharedMutex::try_lock() {
    throw_if_mutex_upgrade();
    return mutex_.try_lock();
}

void SafeRecursiveSharedMutex::lock_shared() {
    if (!mutex_.is_owner()) {
        std::scoped_lock lock{ shared_owners_mutex_ };
        if (++shared_owners_[std::this_thread::get_id()] > 1) {
            return;
        }
    }
    mutex_.lock_shared();
}

void SafeRecursiveSharedMutex::unlock_shared() {
    if (!mutex_.is_owner()) {
        std::scoped_lock lock{ shared_owners_mutex_ };
        auto it = shared_owners_.find(std::this_thread::get_id());
        if (it == shared_owners_.end()) {
            verbose_abort("Could not delete shared mutex info");
        }
        if (it->second == 1) {
            if (shared_owners_.size() > max_map_size_) {
                shared_owners_.erase(std::this_thread::get_id());
            } else {
                it->second = 0;
            }
        } else {
            if (it->second == 0) {
                verbose_abort("Too many mutex unlocks");
            }
            --it->second;
            return;
        }
    }
    mutex_.unlock_shared();
}

void SafeRecursiveSharedMutex::assert_locked() const {
    if (!mutex_.is_owner()) {
        throw std::runtime_error("Mutex not locked");
    }
}

void SafeRecursiveSharedMutex::assert_locked_or_shared() const {
    std::scoped_lock lock{ shared_owners_mutex_ };
    if (!mutex_.is_owner() && !shared_owners_.contains(std::this_thread::get_id())) {
        throw std::runtime_error("Mutex not locked or shared");
    }
}

void SafeRecursiveSharedMutex::throw_if_mutex_upgrade() const {
    std::scoped_lock lock{ shared_owners_mutex_ };
    auto it = shared_owners_.find(std::this_thread::get_id());
    if ((it != shared_owners_.end()) && (it->second != 0)) {
        verbose_abort("Mutex upgrade not supported");
    }
}
