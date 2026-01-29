// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Object_Pool.hpp"
#include <VehicleArena/Memory/Recursive_Deletion.hpp>
#include <VehicleArena/Os/Os.hpp>
#include <VehicleArena/Throw_Or_Abort.hpp>
#include <exception>

using namespace Mlib;

ObjectPool VA::global_object_pool{ InObjectPoolDestructor::ASSERT_NO_LEAKS };

ObjectPool::ObjectPool(InObjectPoolDestructor what_to_do_in_dtor)
    : what_to_do_in_dtor_{ what_to_do_in_dtor }
    , clearing_{ false }
{}

ObjectPool::~ObjectPool() {
    if (!deleting_ptrs_.empty()) {
        verbose_abort("ObjectPool dtor called during deletion");
    }
    switch (what_to_do_in_dtor_) {
    case InObjectPoolDestructor::CLEAR:
        clear();
        break;
    case InObjectPoolDestructor::ASSERT_NO_LEAKS:
        assert_no_leaks();
        break;
    default:
        verbose_abort("Unknown InObjectPoolDestructor value");
    }
}

void ObjectPool::add(std::function<void()> deallocate, Object& o, SourceLocation loc) {
    std::scoped_lock lock{ mutex_ };
    if (clearing_) {
        verbose_abort("ObjectPool::add called during clearing");
    }
    if (!ptrs_.emplace(std::move(deallocate), &o, loc).second) {
        THROW_OR_ABORT("Unique pointer already exists");
    }
}

// void ObjectPool::add(Object* o, SourceLocation loc) {
//     if (o == nullptr) {
//         verbose_abort("Attempt to add nullptr");
//     }
//     add(*o, loc);
// }

bool ObjectPool::contains(Object* o) const {
    if (o == nullptr) {
        verbose_abort("ObjectPool: Attempt to check for nullptr");
    }
    return contains(*o);
}

bool ObjectPool::contains(Object& o) const {
    std::unique_lock lock{ mutex_ };
    return ptrs_.contains({ nullptr, &o, CURRENT_SOURCE_LOCATION });
}

void ObjectPool::remove(Object& o) {
    std::unique_lock lock{ mutex_ };
    if (deleting_ptrs_.contains(&o)) {
        return;
    }
    auto n = ptrs_.extract({ nullptr, &o, CURRENT_SOURCE_LOCATION });
    if (n.empty()) {
        verbose_abort("ObjectPool: Could not remove object");
    }
    lock.unlock();
    delete_(n.value());
}

void ObjectPool::remove(Object* o) {
    if (o == nullptr) {
        verbose_abort("ObjectPool: Attempt to remove nullptr");
    }
    remove(*o);
}

void ObjectPool::extract(Object* o) {
    if (o == nullptr) {
        verbose_abort("ObjectPool: Attempt to extract nullptr");
    }
    extract(*o);
}

void ObjectPool::extract(Object& o) {
    std::unique_lock lock{ mutex_ };
    auto n = ptrs_.extract({ nullptr, &o, CURRENT_SOURCE_LOCATION });
    if (n.empty()) {
        verbose_abort("ObjectPool: Could not extract object");
    }
}

void ObjectPool::delete_(const ObjectAndSourceLocation& o) {
    {
        std::scoped_lock lock{ mutex_ };
        if (!deleting_ptrs_.insert(o.object).second) {
            verbose_abort("Could not insert into deleting_ptrs");
        }
    }
    std::exception_ptr eptr = nullptr;
    try {
        o.object->~Object();
    } catch (...) {
        lwarn() << "Destructor threw an exception";
        eptr = std::current_exception(); 
    }
    {
        std::scoped_lock lock{ mutex_ };
        if (deleting_ptrs_.erase(o.object) != 1) {
            verbose_abort("Could not erase from deleting_ptrs");
        }
    }
    o.deallocate();
    if (eptr != nullptr) {
        std::rethrow_exception(eptr);
    }
}

void ObjectPool::clear() {
    std::unique_lock lock{ mutex_ };
    if (clearing_) {
        verbose_abort("ObjectPool already clearing");
    }
    clearing_ = true;
    clear_set_recursively_with_lock(ptrs_, lock, [this](auto& o) { delete_(o); });
    clearing_ = false;
}

void ObjectPool::assert_no_leaks() const {
    if (!ptrs_.empty()) {
        for (const auto& p : ptrs_) {
            lerr() << p.loc;
        }
        verbose_abort("Memory leaks detected in ObjctPool");
    }
}
