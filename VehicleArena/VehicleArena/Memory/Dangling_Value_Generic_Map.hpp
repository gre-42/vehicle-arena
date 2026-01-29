// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Memory/Destruction_Functions.hpp>
#include <VehicleArena/Memory/Destruction_Functions_Removeal_Tokens_Ref.hpp>
#include <VehicleArena/Source_Location.hpp>
#include <vector>

namespace VA {

template <class T>
class DanglingBaseClassRef;

template <class TMap>
class DanglingValueGenericMap {
public:
    using key_type = TMap::key_type;
    using mapped_type = TMap::mapped_type;
    using value_type = TMap::value_type;
    using iterator = TMap::iterator;
    using const_iterator = TMap::const_iterator;
    using base_mapped_type = mapped_type::value_type;
    DanglingValueGenericMap() = default;
    DanglingValueGenericMap(const DanglingValueGenericMap&) = default;
    ~DanglingValueGenericMap() = default;
    decltype(auto) emplace(const key_type& key, const DanglingBaseClassRef<base_mapped_type>& element, SourceLocation loc) {
        auto it = elements_.try_emplace(key, element, element->on_destroy, loc);
        if (it.second) {
            it.first->second.on_destroy([this, it=it.first](){ elements_.extract(it); }, loc);
        }
        return it;
    }
    template <class TElement>
    decltype(auto) erase(const TElement& v) {
        return elements_.erase(v);
    }
    decltype(auto) begin() { return elements_.begin(); }
    decltype(auto) end() { return elements_.end(); }
    decltype(auto) begin() const { return elements_.begin(); }
    decltype(auto) end() const { return elements_.end(); }
    bool empty() const {
        return elements_.empty();
    }
    std::size_t size() const {
        return elements_.size();
    }
    template <class TKey2>
    bool contains(const TKey2& key) const {
        return elements_.contains(key);
    }
    template <class TKey2>
    decltype(auto) find(const TKey2& key) {
        return elements_.find(key);
    }
    template <class TKey2>
    decltype(auto) find(const TKey2& key) const {
        return elements_.find(key);
    }
private:
    TMap elements_;
};

}
