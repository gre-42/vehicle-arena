// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Threads/Fast_Mutex.hpp>
#include <VehicleArena/Threads/Recursive_Shared_Mutex.hpp>
#include <cstdint>
#include <mutex>
#include <unordered_map>

namespace VA {

class SafeRecursiveSharedMutex {
public:
    explicit SafeRecursiveSharedMutex(size_t max_map_size = 1'000);
    ~SafeRecursiveSharedMutex();
    void lock();
    void unlock();
    bool try_lock();
    void lock_shared();
    void unlock_shared();
    void assert_locked() const;
    void assert_locked_or_shared() const;
private:
    void throw_if_mutex_upgrade() const;
    RecursiveSharedMutex mutex_;
    mutable FastMutex shared_owners_mutex_;
    std::unordered_map<std::thread::id, uint32_t> shared_owners_;
    size_t max_map_size_;
};

}
