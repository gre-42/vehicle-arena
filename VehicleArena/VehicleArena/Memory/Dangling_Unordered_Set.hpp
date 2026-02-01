// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Memory/Destruction_Functions.hpp>
#include <VehicleArena/Memory/Destruction_Functions_Removeal_Tokens_Ptr.hpp>
#include <VehicleArena/Misc/Source_Location.hpp>
#include <unordered_set>
#include <vector>

namespace VA {

template <class T>
class DanglingBaseClassPtr;

template <class T>
class DanglingUnorderedSet {
public:
    using Element = DestructionFunctionsTokensPtr<T>;
    using Elements = std::unordered_set<Element, std::hash<Element>, DestructionFunctionsTokensPtrComparator<T>>;
    DanglingUnorderedSet() = default;
    DanglingUnorderedSet(const DanglingUnorderedSet&) = default;
    ~DanglingUnorderedSet() = default;
    DanglingUnorderedSet(const std::vector<T>& vec, SourceLocation loc) {
        for (const auto& element : vec) {
            emplace(element, loc);
        }
    }
    decltype(auto) emplace(const DanglingBaseClassPtr<T>& element, SourceLocation loc) {
        auto it = elements_.emplace(element, element->on_destroy, loc);
        if (it.second) {
            it.first->on_destroy([this, it=it.first](){ elements_.extract(it); }, loc);
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
    template <class TKey>
    bool contains(const TKey& key) const {
        return elements_.contains(key);
    }
private:
    Elements elements_;
};

}
