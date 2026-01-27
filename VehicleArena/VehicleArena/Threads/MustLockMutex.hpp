#pragma once
#include <mutex>

namespace VA {

class MustLockMutex: public std::timed_mutex {
public:
    bool must_lock() {
        return try_lock_for(std::chrono::milliseconds(500));
    }
};

}
