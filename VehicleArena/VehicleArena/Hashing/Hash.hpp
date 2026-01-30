// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <cstddef>
#include <functional>

namespace VA {

// From: https://stackoverflow.com/questions/2590677/how-do-i-combine-hash-values-in-c0x
class Hasher {
public:
    explicit Hasher(std::size_t seed = 0xc0febabe)
        : seed{ seed }
    {}
    void combine() const {
        // Do nothing
    }
    template <typename T, typename... Rest>
    inline void combine(const T& v, const Rest&... rest) {
        std::hash<T> hasher;
        seed ^= hasher(v) + 0x9e3779b9 + (seed<<6) + (seed>>2);
        combine(rest...);
    }
    operator std::size_t() const {
        return seed;
    }
private:
    std::size_t seed;
};

template <typename... Args>
inline std::size_t hash_combine(const Args&... args) {
    Hasher hasher{ 0xc0febabe };
    hasher.combine(args...);
    return hasher;
}

}
