// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Colored_Vertex_Array_Filter.hpp"
#include <VehicleArena/Geometry/Mesh/Colored_Vertex_Array.hpp>
#include <VehicleArena/Json/Json_View.hpp>
#include <VehicleArena/Misc/Argument_List.hpp>
#include <VehicleArena/Scene_Config/Scene_Precision.hpp>

using namespace VA;

namespace KnownArgs {
BEGIN_ARGUMENT_LIST;
DECLARE_ARGUMENT(included_tags);
DECLARE_ARGUMENT(excluded_tags);
DECLARE_ARGUMENT(included_names);
DECLARE_ARGUMENT(excluded_names);
}

ColoredVertexArrayFilter::~ColoredVertexArrayFilter() = default;

template <class TPos>
bool ColoredVertexArrayFilter::matches(const ColoredVertexArray<TPos>& cva) const {
    auto n = cva.name.name();
    return
        !any(~cva.morphology.physics_material & included_tags) &&
        !any(cva.morphology.physics_material & excluded_tags) &&
        VA::re::regex_search(n.data(), n.data() + n.size(), included_names) &&
        !VA::re::regex_search(n.data(), n.data() + n.size(), excluded_names);
}

void VA::from_json(const nlohmann::json& j, ColoredVertexArrayFilter& filter) {
    JsonView jv{ j };
    jv.validate(KnownArgs::options);
    if (jv.contains(KnownArgs::included_tags)) {
        filter.included_tags = physics_material_from_string(jv.at<std::string>(KnownArgs::included_tags));
    }
    if (jv.contains(KnownArgs::excluded_tags)) {
        filter.excluded_tags = physics_material_from_string(jv.at<std::string>(KnownArgs::excluded_tags));
    }
    if (jv.contains(KnownArgs::included_names)) {
        filter.included_names = VA::compile_regex(jv.at<std::string>(KnownArgs::included_names));
    }
    if (jv.contains(KnownArgs::excluded_names)) {
        filter.excluded_names = VA::compile_regex(jv.at<std::string>(KnownArgs::excluded_names));
    }
}

ColoredVertexArrayFilters::ColoredVertexArrayFilters()
: filters_{ ColoredVertexArrayFilter{} }
{}

ColoredVertexArrayFilters::ColoredVertexArrayFilters(std::vector<ColoredVertexArrayFilter> filters)
    : filters_{ std::move(filters) }
{}

template <class TPos>
bool ColoredVertexArrayFilters::matches(const ColoredVertexArray<TPos>& cva) const {
    for (const auto& f : filters_) {
        if (f.matches(cva)) {
            return true;
        }
    }
    return false;
}

void VA::from_json(const nlohmann::json& j, ColoredVertexArrayFilters& filters) {
    if (j.type() != nlohmann::detail::value_t::array) {
        throw std::runtime_error("Type is not array for ColoredVertexArrayFilters");
    }
    filters = ColoredVertexArrayFilters{ j.get<std::vector<ColoredVertexArrayFilter>>() };
}

template bool ColoredVertexArrayFilter::matches(const ColoredVertexArray<float>& cva) const;
template bool ColoredVertexArrayFilter::matches(const ColoredVertexArray<CompressedScenePos>& cva) const;

template bool ColoredVertexArrayFilters::matches(const ColoredVertexArray<float>& cva) const;
template bool ColoredVertexArrayFilters::matches(const ColoredVertexArray<CompressedScenePos>& cva) const;
