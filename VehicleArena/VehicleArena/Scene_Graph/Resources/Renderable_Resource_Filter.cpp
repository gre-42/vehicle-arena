// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Renderable_Resource_Filter.hpp"
#include <VehicleArena/Geometry/Mesh/Colored_Vertex_Array.hpp>
#include <VehicleArena/Geometry/Physics_Material.hpp>

using namespace VA;

RenderableResourceFilter::~RenderableResourceFilter()
{}

template <class TPos>
bool RenderableResourceFilter::matches(size_t num, const ColoredVertexArray<TPos>& cva) const {
    return
        (num >= min_num) &&
        (num <= max_num) &&
        cva_filter.matches(cva);
}

std::ostream& VA::operator << (std::ostream& ostr, const RenderableResourceFilter& filter) {
    ostr << "RenderableResourceFilter\n";
    ostr << "  min_num: " << filter.min_num << '\n';
    ostr << "  max_num: " << filter.max_num << '\n';
    ostr << "  included tags: " << physics_material_to_string(filter.cva_filter.included_tags) << '\n';
    ostr << "  excluded tags: " << physics_material_to_string(filter.cva_filter.excluded_tags) << '\n';
    ostr << "  included names: <regex>\n";
    ostr << "  excluded names: <regex>\n";
    return ostr;
}

template bool RenderableResourceFilter::matches(size_t num, const ColoredVertexArray<float>& cva) const;
template bool RenderableResourceFilter::matches(size_t num, const ColoredVertexArray<CompressedScenePos>& cva) const;
