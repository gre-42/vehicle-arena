// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Hashing/Variable_And_Hash.hpp>
#include <VehicleArena/Math/Orderable_Fixed_Array.hpp>
#include <cstdint>
#include <iosfwd>
#include <string>
#include <vector>

namespace VA {

enum class InteriorTextureSet: uint32_t;

static const size_t INTERIOR_LEFT = 0;
static const size_t INTERIOR_RIGHT = 1;
static const size_t INTERIOR_FLOOR = 2;
static const size_t INTERIOR_CEILING = 3;
static const size_t INTERIOR_BACK = 4;
// back_specular        5
// front_color          6
// front_alpha          7
// front_specular       8
static const size_t INTERIOR_COUNT_MAX = 9;

struct InteriorTextures {
    InteriorTextures();
    InteriorTextures(const InteriorTextures& other);
    InteriorTextures(InteriorTextures&& other);
    InteriorTextures& operator = (const InteriorTextures& other);
    InteriorTextures& operator = (InteriorTextures&& other);
    std::strong_ordering operator <=> (const InteriorTextures&) const = default;
    bool empty() const;
    size_t size() const;
    const VariableAndHash<std::string>& operator [](size_t index) const;
    template <class Archive>
    void serialize(Archive& archive) {
        archive(facade_edge_size);
        archive(facade_inner_size);
        archive(interior_size);
        archive(names);
        archive(set);
    }
    OrderableFixedArray<float, 2> facade_edge_size{ 0.f, 0.f };
    OrderableFixedArray<float, 2> facade_inner_size{ 0.f, 0.f };
    OrderableFixedArray<float, 3> interior_size{ 0.f, 0.f, 0.f };
    std::vector<VariableAndHash<std::string>> names;
    InteriorTextureSet set;
    void assign(
        VariableAndHash<std::string> left,
        VariableAndHash<std::string> right,
        VariableAndHash<std::string> floor,
        VariableAndHash<std::string> ceiling,
        VariableAndHash<std::string> back,
        VariableAndHash<std::string> back_specular,
        VariableAndHash<std::string> front,
        VariableAndHash<std::string> front_alpha,
        VariableAndHash<std::string> front_specular);
};

std::ostream& operator << (std::ostream& ostr, const InteriorTextures& t);

}
