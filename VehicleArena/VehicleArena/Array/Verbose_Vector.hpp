// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Throw_Or_Abort.hpp>
#include <cstddef>
#include <string>
#include <vector>

namespace VA {

template <class T>
class VerboseVector {
public:
    VerboseVector(std::string name)
        : name_{ std::move(name) }
    {}
    void reserve(size_t n) {
        data_.reserve(n);
    }
    template <class... Args>
    decltype(auto) emplace_back(Args&&... args) {
        return data_.emplace_back(std::forward<Args>(args)...);
    }
    decltype(auto) get(size_t i) {
        if (i >= data_.size()) {
            THROW_OR_ABORT(
                name_ + ": index (" + std::to_string(i) +
                ") is out of bounds (" + std::to_string(data_.size()) + ')');
        }
        return data_[i];
    }
    decltype(auto) get(size_t i) const {
        return const_cast<VerboseVector<T>&>(*this).get(i);
    }
    size_t size() const {
        return data_.size();
    }
private:
    std::string name_;
    std::vector<T> data_;
};
    
}
