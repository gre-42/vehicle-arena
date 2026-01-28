#include <VehicleArena/Threads/FastMutex.hpp>
#include <VehicleArena/Threads/GuardedObject.hpp>
#include <VehicleArena/Threads/ThrowingGuardedObject.hpp>
#include <VehicleArena/Threads/ThrowingLockGuard.hpp>
#include <iostream>

using namespace VA;

class GeneralProtectedClass {
    template <class TObject, class TLock>
    friend class VA::GeneralGuardedObject;
    GeneralProtectedClass() = default;
public:
    template <class... Args>
    static GeneralGuardedObject<GeneralProtectedClass, ThrowingLockGuard<FastMutex>> create(Args&&... args) {
        return {std::forward<Args>(args)...};
    }
    void func() {
        std::cerr << "func()" << std::endl;
    }
private:
    FastMutex mutex_;
};

class SimpleProtectedClass {
public:
    void func() {
        std::cerr << "func()" << std::endl;
    }
};

void test_general_protected_class() {
    auto c = GeneralProtectedClass::create();
    auto l = c.lock();
    l->func();
}

void test_simple_protected_class() {
    auto c = ThrowingGuardedObject<SimpleProtectedClass>();
    auto l = c.lock();
    l->func();
}

int main() {
    try {
        test_general_protected_class();
        test_simple_protected_class();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
