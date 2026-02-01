// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Memory/Destruction_Functions.hpp>
#include <VehicleArena/Misc/Source_Location.hpp>
#include <map>

namespace VA {

template <class T>
class DanglingMap {
public:
    template <class TKey>
    decltype(auto) try_emplace(const TKey& key, SourceLocation loc) {
        auto res = elements_.try_emplace(key, key->on_destroy, loc);
        if (!res.second) {
            return res;
        }
        res.first->second.add([this, it=res.first](){ elements_.extract(it); }, loc);
        return res;
    }
    template <class TKey>
    decltype(auto) erase(const TKey& v) {
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
private:
    std::map<DanglingBaseClassPtr<T>, DestructionFunctionsRemovalTokens> elements_;
};

}
