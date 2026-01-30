#include <VehicleArena/Os/Os.hpp>
#include <VehicleArena/Threads/Fast_Mutex.hpp>
#include <VehicleArena/Threads/Guarded_Object.hpp>
#include <VehicleArena/Threads/Throwing_Guarded_Object.hpp>
#include <VehicleArena/Threads/Throwing_Lock_Guard.hpp>

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
        linfo() << "func()";
    }
private:
    FastMutex mutex_;
};

class SimpleProtectedClass {
public:
    void func() {
        linfo() << "func()";
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
        lerr() << "Error: " << e.what();
        return 1;
    }
    return 0;
}
