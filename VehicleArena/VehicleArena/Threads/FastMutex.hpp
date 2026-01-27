#pragma once
#include <VehicleArena/Threads/AtomicMutex.hpp>
#include <mutex>

namespace VA {

#ifdef __SANITIZE_THREAD__
using FastMutex = std::mutex;
#else
using FastMutex = AtomicMutex;
#endif

}
