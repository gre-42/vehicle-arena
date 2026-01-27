#include <VehicleArena/Threads/FastMutex.hpp>
#include <VehicleArena/Threads/GuardedObject.hpp>
#include <VehicleArena/Threads/ThrowingLockGuard.hpp>
#include <iostream>

using namespace VA;

class ProtectedClass {
    template <class TObject, class TLock>
    friend class VA::GuardedObject;
    ProtectedClass() = default;
public:
    template <class... Args>
    static GuardedObject<ProtectedClass, ThrowingLockGuard<FastMutex>> create(Args&&... args) {
        return {std::forward<Args>(args)...};
    }
    void func() {
        std::cerr << "func()" << std::endl;
    }
private:
    FastMutex mutex_;
};

void test_non_copyable_shared_ptr() {
    auto c = ProtectedClass::create();
    auto l = c.lock();
    l->func();
}

int main() {
    test_non_copyable_shared_ptr();
}
