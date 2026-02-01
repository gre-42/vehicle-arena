// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Color_Style.hpp"
#include <VehicleArena/Hashing/Hash.hpp>

using namespace VA;

void ColorStyle::insert(const ColorStyle& other) {
    std::scoped_lock lock{ parameter_mutex_ };
    if (!other.emissive.all_equal(-1.f)) {
        this->emissive = other.emissive;
    }
    if (!other.ambient.all_equal(-1.f)) {
        this->ambient = other.ambient;
    }
    if (!other.diffuse.all_equal(-1.f)) {
        this->diffuse = other.diffuse;
    }
    if (!other.specular.all_equal(-1.f)) {
        this->specular = other.specular;
    }
    if (!other.fresnel_ambient.all_equal(-1.f)) {
        this->fresnel_ambient = other.fresnel_ambient;
    }
    if (other.fresnel.exponent != -1.f) {
        this->fresnel = other.fresnel;
    }
    if (other.reflection_strength != -1.f) {
        this->reflection_strength = other.reflection_strength;
    }
    for (const auto& [key, value] : other.reflection_maps) {
        this->reflection_maps[key] = value;
    }
}

bool ColorStyle::matches(const VariableAndHash<std::string>& name) const {
    return matches_.get(name);
}

size_t ColorStyle::get_hash() const {
    std::scoped_lock lock{ hash_mutex_ };
    return hash_.get();
}

void ColorStyle::update_hash() {
    std::scoped_lock lock{ hash_mutex_ };
    hash_.reset();
    compute_hash();
}

ColorStyle& ColorStyle::compute_hash() {
    Hasher hasher{ 0xc0febabe };
    hasher.combine(
        emissive,
        ambient,
        diffuse,
        specular,
        specular_exponent,
        fresnel_ambient,
        fresnel,
        reflection_strength);
    hasher.combine(reflection_maps.size());
    for (const auto& [k, v] : reflection_maps) {
        hasher.combine(k, v);
    }
    hash_ = hasher;
    return *this;
}

void ColorStyle::set_emissive(const FixedArray<float, 3>& value) {
    std::scoped_lock lock{ parameter_mutex_, hash_mutex_ };
    emissive = value;
    hash_.reset();
    compute_hash();
}
