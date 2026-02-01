// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Hashing/Variable_And_Hash.hpp>
#include <VehicleArena/Iterator/Guarded_Iterable.hpp>
#include <VehicleArena/Threads/Recursive_Shared_Mutex.hpp>
#include <functional>
#include <mutex>
#include <stdexcept>
#include <string>
#include <unordered_map>

namespace VA {

template <class T>
class ThreadsafeDefaultMap {
    using TBaseMap = std::unordered_map<VariableAndHash<std::string>, T>;
    using TMutex = SafeAtomicRecursiveSharedMutex;
public:
    explicit ThreadsafeDefaultMap(
        std::function<T(const VariableAndHash<std::string>&)> deflt)
        : default_{ std::move(deflt) }
    {}

    void insert(const VariableAndHash<std::string>& name, const T& value) {
        std::scoped_lock lock{mutex_};
        if (!elements_.insert({name, value}).second) {
            throw std::runtime_error("Element with name \"" + *name + "\" already exists");
        }
    }

    T& get(const VariableAndHash<std::string>& name) {
        {
            std::shared_lock lock{mutex_};
            auto it = elements_.find(name);
            if (it != elements_.end()) {
                return it->second;
            }
        }
        std::scoped_lock lock{mutex_};
        auto it = elements_.find(name);
        if (it != elements_.end()) {
            return it->second;
        }
        auto iit = elements_.insert({name, default_(name)});
        if (!iit.second) {
            throw std::runtime_error("Recursive insertion: Element with name \"" + *name + "\" already exists");
        }
        return iit.first->second;
    }

    const T& get(const VariableAndHash<std::string>& name) const {
        return const_cast<ThreadsafeDefaultMap*>(this)->get(name);
    }

    GuardedIterable<typename TBaseMap::iterator, std::scoped_lock<TMutex>> scoped() {
        return { mutex_, elements_.begin(), elements_.end() };
    }
    GuardedIterable<typename TBaseMap::iterator, std::shared_lock<TMutex>> shared() {
        return { mutex_, elements_.begin(), elements_.end() };
    }
    GuardedIterable<typename TBaseMap::const_iterator, std::scoped_lock<TMutex>> scoped() const {
        return { mutex_, elements_.begin(), elements_.end() };
    }
    GuardedIterable<typename TBaseMap::const_iterator, std::shared_lock<TMutex>> shared() const {
        return { mutex_, elements_.begin(), elements_.end() };
    }
private:
    mutable TMutex mutex_;
    TBaseMap elements_;
    std::function<T(const VariableAndHash<std::string>&)> default_;
};

}
