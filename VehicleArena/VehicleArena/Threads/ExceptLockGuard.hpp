#pragma once
#include <stdexcept>

namespace VA {

template <class TMutex>
class ExceptLockGuard {
public:
    explicit ExceptLockGuard(TMutex& mutex)
        : mutex_{mutex}
    {
        if (!mutex_.must_lock()) {
            throw std::runtime_error("Could not lock mutex");
        }
    }
    ~ExceptLockGuard() {
        mutex_.unlock();
    }
private:
    TMutex& mutex_;
};

}
