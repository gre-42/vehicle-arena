// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Interior_Textures.hpp"
#include <VehicleArena/Geometry/Material/Interior_Texture_Set.hpp>
#include <VehicleArena/Regex/Regex_Select.hpp>
#include <VehicleArena/Strings/To_Number.hpp>
#include <ostream>
#include <stdexcept>
#include <stdexcept>

using namespace VA;

InteriorTextures::InteriorTextures()
    : set{ InteriorTextureSet::NONE }
{}

InteriorTextures::InteriorTextures(const InteriorTextures& other) = default;
InteriorTextures::InteriorTextures(InteriorTextures&& other) = default;
InteriorTextures& InteriorTextures::operator = (const InteriorTextures& other) = default;
InteriorTextures& InteriorTextures::operator = (InteriorTextures&& other) = default;

bool InteriorTextures::empty() const {
    return names.empty();
}

size_t InteriorTextures::size() const {
    return names.size();
}

const VariableAndHash<std::string>& InteriorTextures::operator [](size_t index) const {
    if (index >= names.size()) {
        throw std::runtime_error("Interior texture index out of bounds");
    }
    return names[index];
}

void InteriorTextures::assign(
    VariableAndHash<std::string> left,
    VariableAndHash<std::string> right,
    VariableAndHash<std::string> floor,
    VariableAndHash<std::string> ceiling,
    VariableAndHash<std::string> back,
    VariableAndHash<std::string> back_specular,
    VariableAndHash<std::string> front,
    VariableAndHash<std::string> front_alpha,
    VariableAndHash<std::string> front_specular)
{
    if (left->empty() || right->empty() || floor->empty() || ceiling->empty() || back->empty()) {
        throw std::runtime_error("Interior color texture is empty");
    }
    names.reserve(5 + !back_specular->empty() + !front->empty() + !front_alpha->empty() + !front_specular->empty());
    names.emplace_back(std::move(left));
    names.emplace_back(std::move(right));
    names.emplace_back(std::move(floor));
    names.emplace_back(std::move(ceiling));
    names.emplace_back(std::move(back));
    set |= InteriorTextureSet::INTERIOR_COLORS;
    if (!back_specular->empty()) {
        set |= InteriorTextureSet::BACK_SPECULAR;
        names.emplace_back(std::move(back_specular));
    }
    if (!front->empty()) {
        set |= InteriorTextureSet::FRONT_COLOR;
        names.emplace_back(std::move(front));
    }
    if (!front_alpha->empty()) {
        set |= InteriorTextureSet::FRONT_ALPHA;
        names.emplace_back(std::move(front_alpha));
    }
    if (!front_specular->empty()) {
        set |= InteriorTextureSet::FRONT_SPECULAR;
        names.emplace_back(std::move(front_specular));
    }
}

std::ostream& VA::operator << (std::ostream& ostr, const InteriorTextures& t) {
    ostr <<
        "facade_edge_size: " << t.facade_edge_size << '\n' <<
        "facade_inner_size: " << t.facade_inner_size << '\n' <<
        "interior_size: " << t.interior_size << '\n';
    for (size_t i = 0; i < t.size(); ++i) {
        ostr << i << ": " << *t[i] << '\n';
    }
    return ostr;
}
