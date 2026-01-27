#pragma once
#include <VehicleArena/Threads/AtomicMutex.hpp>
#include <VehicleArena/Threads/MustLockMutex.hpp>
#include <mutex>

namespace VA {

#ifdef __SANITIZE_THREAD__
using FastMutex = MustLockMutex;
#else
using FastMutex = AtomicMutex;
#endif

}
