// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Array/Fixed_Array.hpp>
#include <VehicleArena/Geometry/Material/Fresnel.hpp>
#include <VehicleArena/Hashing/Cached_Hash.hpp>
#include <VehicleArena/Hashing/Variable_And_Hash.hpp>
#include <VehicleArena/Map/Threadsafe_Default_Map.hpp>
#include <VehicleArena/Regex/Regex_Select.hpp>
#include <VehicleArena/Threads/Fast_Mutex.hpp>
#include <optional>
#include <unordered_map>

namespace VA {

/**
 * Don't forget to update the "insert" function when adding new fields.
 */
struct ColorStyle {
    std::optional<VA::re::cregex> selector;
    OrderableFixedArray<float, 3> emissive{-1.f, -1.f, -1.f};
    OrderableFixedArray<float, 3> ambient{-1.f, -1.f, -1.f};
    OrderableFixedArray<float, 3> diffuse{-1.f, -1.f, -1.f};
    OrderableFixedArray<float, 3> specular{-1.f, -1.f, -1.f};
    float specular_exponent = -1.f;
    OrderableFixedArray<float, 3> fresnel_ambient{-1.f, -1.f, -1.f};
    FresnelReflectance fresnel{
        .min = -1.f,
        .max = -1.f,
        .exponent = -1.f
    };
    std::unordered_map<VariableAndHash<std::string>, VariableAndHash<std::string>> reflection_maps;
    float reflection_strength = 1.f;
    mutable FastMutex parameter_mutex_;
    CachedHash hash_;
    mutable FastMutex hash_mutex_;
    size_t get_hash() const;
    void insert(const ColorStyle& other);
    bool matches(const VariableAndHash<std::string>& name) const;
    void update_hash();
    ColorStyle& compute_hash();
    mutable ThreadsafeDefaultMap<bool> matches_{ [this](const VariableAndHash<std::string>& name){
        return
            !selector.has_value() ||
            VA::re::regex_search(*name, *selector);
    } };
    void set_emissive(const FixedArray<float, 3>& value);
};

}
