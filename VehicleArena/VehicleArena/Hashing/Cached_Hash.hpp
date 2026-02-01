// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <compare>
#include <cstdint>
#include <optional>
#include <stdexcept>

namespace VA {

class CachedHash {
public:
    inline CachedHash& operator = (size_t value) {
        if (value_.has_value()) {
            throw std::runtime_error("Hash already computed");
        }
        value_ = value;
        return *this;
    }
    inline size_t operator * () const {
        return *value_;
    }
    inline size_t get() const {
        if (!value_.has_value()) {
            throw std::runtime_error("Hash not computed");
        }
        return *value_;
    }
    inline void reset() {
        value_.reset();
    }
    inline bool has_value() const {
        return value_.has_value();
    }
    bool operator < (const CachedHash&) const {
        return false;
    }
    bool operator == (const CachedHash&) const {
        return true;
    }
    bool operator != (const CachedHash&) const {
        return false;
    }
    inline bool operator > (const CachedHash&) const {
        return false;
    }
    inline std::strong_ordering operator <=> (const CachedHash&) const {
        return std::strong_ordering::equal;
    }
    template <class Archive>
    void serialize(Archive& archive) {
        archive(value_);
    }
private:
    std::optional<size_t> value_;
};

}
