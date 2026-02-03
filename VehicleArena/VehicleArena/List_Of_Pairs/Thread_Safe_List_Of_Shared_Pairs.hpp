// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/List_Of_Pairs/Readonly_List_Of_Shared_Pairs.hpp>
#include <VehicleArena/Threads/Recursive_Shared_Mutex.hpp>
#include <mutex>

namespace VA {

template <class TMutex, class TBaseList>
class GenericThreadSafeListOfSharedPairs {
public:
    using value_type = TBaseList::value_type;

    GenericThreadSafeListOfSharedPairs() = default;
    ~GenericThreadSafeListOfSharedPairs() = default;
    template <class... TArgs>
    decltype(auto) emplace_back(TArgs&&... args) {
        std::scoped_lock lock{ mutex_ };
        return container_.container_.emplace_back(std::forward<TArgs>(args)...);
    }
    template <class TArgs>
    void push_back(TArgs&& arg) {
        std::scoped_lock lock{ mutex_ };
        container_.container_.push_back(std::forward<TArgs>(arg));
    }
    void clear() {
        std::scoped_lock lock{ mutex_ };
        container_.container_.clear();
    }
    TBaseList iterable() {
        std::shared_lock lock{ mutex_ };
        return container_;
    }
    const TBaseList iterable() const {
        return const_cast<GenericThreadSafeListOfSharedPairs*>(this)->iterable();
    }
    bool empty() const {
        return container_.container_.empty();
    }
    std::size_t size() const {
        return container_.container_.size();
    }
private:
    mutable TMutex mutex_;
    TBaseList container_;
};

template <class First, class Second>
using ThreadSafeListOfSharedPairs = GenericThreadSafeListOfSharedPairs<SafeAtomicRecursiveSharedMutex, ReadonlyListOfSharedPairs<First, Second>>;

}
