// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Os/Os.hpp>
#include <cstddef>
#include <list>
#include <map>
#include <unordered_map>

namespace VA {

template <class TKey, class TValue>
struct ReferenceMapElement {
    const TKey& key;
    TValue& value;
};

template <class TList, class TMap>
class OrderedMap {
public:
    using key_type = TMap::key_type;
    using value_type = TMap::value_type;    
    using mapped_type = TMap::mapped_type;
    using iterator = TList::iterator;
    OrderedMap() = default;
    OrderedMap(TMap map)
        : map_{ std::move(map) }
    {}
    template <class... Args>
    decltype(auto) try_emplace(const key_type& key, Args&&... args) {
        auto res = map_.try_emplace(key, std::forward<Args>(args)...);
        if (!res.second) {
            return res;
        }
        elements_.emplace_back(res.first->first, res.first->second);
        return res;
    }
    template <class TKey, class... Args>
    decltype(auto) try_emplace(TKey&& key, Args&&... args) {
        auto res = map_.try_emplace(std::forward<TKey>(key), std::forward<Args>(args)...);
        if (!res.second) {
            return res;
        }
        elements_.emplace_back(res.first->first, res.first->second);
        return res;
    }
    decltype(auto) at(const key_type& k) {
        return map_.at(k);
    }
    decltype(auto) begin() const {
        return elements_.begin();
    }
    decltype(auto) end() const {
        return elements_.end();
    }
    size_t size() const {
        return map_.size();
    }
    bool empty() const {
        return map_.empty();
    }
    void clear() {
        map_.clear();
        elements_.clear();
    }
private:
    TMap map_;
    TList elements_;
};

template <class TKey, class TValue>
using OrderedUnorderedMap = OrderedMap<std::list<ReferenceMapElement<TKey, TValue>>, std::unordered_map<TKey, TValue>>;

template <class TKey, class TValue>
using OrderedStandardMap = OrderedMap<std::list<ReferenceMapElement<TKey, TValue>>, std::map<TKey, TValue>>;

}
