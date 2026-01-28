#pragma once
#include <VehicleArena/Threads/FastMutex.hpp>
#include <VehicleArena/Threads/GuardedObject.hpp>
#include <VehicleArena/Threads/ThrowingLockGuard.hpp>

namespace VA {

template <class T>
using ThrowingGuardedObject = SimpleGuardedObject<T, FastMutex, ThrowingLockGuard<FastMutex>>;

}
