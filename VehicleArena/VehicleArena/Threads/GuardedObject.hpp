#pragma once
#include <memory>

namespace VA {

template <class TObject, class TLock>
class LockedGuardedObject {
    LockedGuardedObject(const LockedGuardedObject&) = delete;
    LockedGuardedObject& operator = (const LockedGuardedObject&) = delete;
public:
    template <class TMutex>
    LockedGuardedObject(TObject& object, TMutex& mutex)
        : object_{object}
        , lock_{mutex}
    {}
    TObject* operator -> () const {
        return &object_;
    }
private:
    TObject& object_;
    TLock lock_;
};

template <class TObject, class TLock>
class GuardedObject {
    GuardedObject(const GuardedObject&) = delete;
    GuardedObject& operator = (const GuardedObject&) = delete;
public:
    template <class... TArgs>
    GuardedObject(TArgs&&... args)
        : object_{ std::forward<TArgs>(args)... }
    {}
    LockedGuardedObject<TObject, TLock> lock() {
        return LockedGuardedObject<TObject, TLock>{object_, object_.mutex_};
    }
private:
    TObject object_;
};

}
