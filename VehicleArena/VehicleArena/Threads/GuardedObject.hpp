#pragma once
#include <memory>

namespace VA {

template <class TObject, class TLock>
class GuardedObject {
    GuardedObject(const GuardedObject&) = delete;
    GuardedObject& operator = (const GuardedObject&) = delete;
public:
    template <class TMutex>
    GuardedObject(TObject& object, TMutex& mutex)
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
class GuardedObjectFactory {
    GuardedObjectFactory(const GuardedObjectFactory&) = delete;
    GuardedObjectFactory& operator = (const GuardedObjectFactory&) = delete;
public:
    template <class... TArgs>
    GuardedObjectFactory(TArgs&&... args)
        : object_{ std::forward<TArgs>(args)... }
    {}
    GuardedObject<TObject, TLock> lock() {
        return GuardedObject<TObject, TLock>{object_, object_.mutex_};
    }
private:
    TObject object_;
};

}
