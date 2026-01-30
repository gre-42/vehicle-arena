// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Threads/Safe_Atomic_Shared_Mutex.hpp>
#include <mutex>
#include <shared_mutex>
#include <stdexcept>

namespace VA {

template <class TObject, class TLock>
class LockedObject {
public:
    template <class TMutex>
    LockedObject(TObject& o, TMutex& mutex)
        : o_{ o }
        , lock_{ mutex }
    {}
    template <class TMutex>
    LockedObject(TObject& o, TMutex& mutex, std::defer_lock_t)
        : o_{ o }
        , lock_{ mutex, std::defer_lock }
    {}
    TObject* operator -> () const {
        return &o_;
    }
    TObject& operator * () const {
        return o_;
    }
    template <class Rep, class Period>
    bool try_lock_for(const std::chrono::duration<Rep,Period>& duration) {
        return lock_.try_lock_for(duration);
    }
private:
    TObject& o_;
    TLock lock_;
};

template <class T>
class LockableObject {
public:
    using LockExclusive = LockedObject<T, std::unique_lock<SafeAtomicSharedMutex>>;
    using ConstLockExclusive = LockedObject<const T, std::unique_lock<SafeAtomicSharedMutex>>;
    using LockShared = LockedObject<T, std::shared_lock<SafeAtomicSharedMutex>>;
    using ConstLockShared = LockedObject<const T, std::shared_lock<SafeAtomicSharedMutex>>;
    template <class Rep, class Period>
    LockExclusive lock_exclusive_for(
        const std::chrono::duration<Rep, Period>& duration,
        const std::string& message)
    {
        LockExclusive result{ o_, mutex_, std::defer_lock };
        if (!result.try_lock_for(duration)) {
            throw std::runtime_error(message + " object is already locked");
        }
        return result;
    }
    template <class Rep, class Period>
    ConstLockExclusive lock_exclusive_for(
        const std::chrono::duration<Rep, Period>& duration,
        const std::string& message) const
    {
        ConstLockExclusive result{ o_, mutex_, std::defer_lock };
        if (!result.try_lock_for(duration)) {
            throw std::runtime_error(message + " object is already locked");
        }
        return result;
    }
    LockShared lock_shared() {
        return { o_, mutex_ };
    }
    ConstLockShared lock_shared() const {
        return { o_, mutex_ };
    }
private:
    T o_;
    mutable SafeAtomicSharedMutex mutex_;
};

}
