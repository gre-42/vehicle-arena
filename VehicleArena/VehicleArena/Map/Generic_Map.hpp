// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Os/Os.hpp>
#include <compare>
#include <map>
#include <sstream>
#include <stdexcept>
#include <stdexcept>
#include <vector>

namespace VA {

template <class TBaseMap>
class GenericMap: public TBaseMap {
public:
    using value_type = TBaseMap::value_type;
    using key_type = TBaseMap::key_type;
    using mapped_type = TBaseMap::mapped_type;
    using node_type = TBaseMap::node_type;

    GenericMap() {}
    GenericMap(std::initializer_list<value_type> init)
        : TBaseMap(std::move(init))
    {}

    bool contains(const key_type& key) const {
        const TBaseMap* m = this;
        return m->contains(key);
    }

    bool contains(const key_type& key, const mapped_type& value, bool deflt = false) const {
        auto it = this->find(key);
        if (it == this->end()) {
            return deflt;
        } else {
            return it->second == value;
        }
    }

    const mapped_type& get(const key_type& key) const {
        return const_cast<GenericMap*>(this)->get(key);
    }

    mapped_type& get(const key_type& key) {
        auto it = this->find(key);
        if (it == this->end()) {
            throw std::runtime_error((std::stringstream() << "Could not find entry with key \"" << key << '"').str());
        }
        return it->second;
    }

    mapped_type get(const key_type& key, const mapped_type& deflt) const {
        auto it = this->find(key);
        if (it == this->end()) {
            return deflt;
        }
        return it->second;
    }

    const mapped_type* try_get(const key_type& key) const {
        auto it = this->find(key);
        if (it == this->end()) {
            return nullptr;
        }
        return &it->second;
    }

    template< class... Args >
    mapped_type& add(const key_type& key, Args&&... args) {
        auto res = this->try_emplace(key, std::forward<Args>(args)...);
        if (!res.second) {
            throw std::runtime_error("Could not insert into map");
        }
        return res.first->second;
    }

    void remove(const key_type& key) {
        if (this->erase(key) != 1) {
            verbose_abort("Could not remove element");
        }
    }

    std::vector<key_type> keys() const {
        std::vector<key_type> result;
        result.reserve(this->size());
        for (const auto& [k, _] : *this) {
            result.push_back(k);
        }
        return result;
    }

    decltype(auto) operator <=> (const GenericMap& other) const {
        const TBaseMap& a = *this;
        const TBaseMap& b = other;
        return a <=> b;
    }
};

}
