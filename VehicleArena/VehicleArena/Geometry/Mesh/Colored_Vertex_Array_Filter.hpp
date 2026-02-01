// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Geometry/Physics_Material.hpp>
#include <VehicleArena/Regex/Default_Regex.hpp>
#include <VehicleArena/Regex/Regex_Select.hpp>
#include <nlohmann/json_fwd.hpp>
#include <vector>

namespace VA {

template <class TPos>
class ColoredVertexArray;

struct ColoredVertexArrayFilter {
    ~ColoredVertexArrayFilter();
    PhysicsMaterial included_tags = PhysicsMaterial::NONE;
    PhysicsMaterial excluded_tags = PhysicsMaterial::NONE;
    VA::re::cregex included_names = ALWAYS;
    VA::re::cregex excluded_names = NEVER;
    template <class TPos>
    bool matches(const ColoredVertexArray<TPos>& cva) const;
};

class ColoredVertexArrayFilters {
public:
    ColoredVertexArrayFilters();
    ColoredVertexArrayFilters(std::vector<ColoredVertexArrayFilter> filters);
    template <class TPos>
    bool matches(const ColoredVertexArray<TPos>& cva) const;
private:
    std::vector<ColoredVertexArrayFilter> filters_;
};

void from_json(const nlohmann::json& j, ColoredVertexArrayFilter& filter);
void from_json(const nlohmann::json& j, ColoredVertexArrayFilters& filters);

}
