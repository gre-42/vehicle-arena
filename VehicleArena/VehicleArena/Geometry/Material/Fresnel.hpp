// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Hashing/Std_Hash.hpp>
#include <VehicleArena/Math/Orderable_Fixed_Array.hpp>
#include <compare>
#include <nlohmann/json_fwd.hpp>

namespace VA {

struct FresnelReflectance {
    float min = 0.f;
    float max = 0.f;
    float exponent = 0.f;
    template <class Archive>
    void serialize(Archive& archive) {
        archive(min);
        archive(max);
        archive(exponent);
    }
    std::partial_ordering operator <=> (const FresnelReflectance&) const = default;
};

struct FresnelAndAmbient {
    FresnelReflectance reflectance;
    OrderableFixedArray<float, 3> ambient{ 0.f, 0.f, 0.f };
    template <class Archive>
    void serialize(Archive& archive) {
        archive(reflectance);
        archive(ambient);
    }
    std::partial_ordering operator <=> (const FresnelAndAmbient&) const = default;
};

void from_json(const nlohmann::json& j, FresnelAndAmbient& f);

}

template <>
struct std::hash<VA::FresnelReflectance>
{
    std::size_t operator() (const VA::FresnelReflectance& a) const {
        return VA::hash_combine(
            a.min,
            a.max,
            a.exponent);
    }
};

template <>
struct std::hash<VA::FresnelAndAmbient>
{
    std::size_t operator() (const VA::FresnelAndAmbient& a) const {
        return VA::hash_combine(
            a.reflectance,
            a.ambient);
    }
};
