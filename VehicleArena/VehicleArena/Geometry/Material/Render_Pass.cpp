// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Render_Pass.hpp"
#include <map>
#include <ostream>
#include <stdexcept>

using namespace VA;

ExternalRenderPassType VA::external_render_pass_type_from_string(const std::string& str) {
    static const std::map<std::string, ExternalRenderPassType> m{
        {"none", ExternalRenderPassType::NONE},
        {"standard", ExternalRenderPassType::STANDARD},
        {"dirtmap", ExternalRenderPassType::DIRTMAP},
        {"lightmap_depth", ExternalRenderPassType::LIGHTMAP_DEPTH},
        {"lightmap_global_static", ExternalRenderPassType::LIGHTMAP_GLOBAL_STATIC},
        {"lightmap_global_dynamic", ExternalRenderPassType::LIGHTMAP_GLOBAL_DYNAMIC},
        {"lightmap_black_global_static", ExternalRenderPassType::LIGHTMAP_BLACK_GLOBAL_STATIC},
        {"lightmap_black_local_instances", ExternalRenderPassType::LIGHTMAP_BLACK_LOCAL_INSTANCES},
        {"lightmap_black_movables", ExternalRenderPassType::LIGHTMAP_BLACK_MOVABLES},
        {"lightmap_black_node", ExternalRenderPassType::LIGHTMAP_BLACK_NODE},
        {"lightmap_black_global_and_local", ExternalRenderPassType::LIGHTMAP_BLACK_GLOBAL_AND_LOCAL},
        {"lightmap_blobs", ExternalRenderPassType::LIGHTMAP_BLOBS}
    };
    auto it = m.find(str);
    if (it == m.end()) {
        throw std::runtime_error("Unknown render pass type: \"" + str + '"');
    }
    return it->second;
}

std::string VA::external_render_pass_type_to_string(ExternalRenderPassType pass) {
    switch (pass) {
    case ExternalRenderPassType::STANDARD: return "standard";
    case ExternalRenderPassType::DIRTMAP: return "dirtmap";
    case ExternalRenderPassType::LIGHTMAP_DEPTH: return "lightmap_depth";
    case ExternalRenderPassType::LIGHTMAP_GLOBAL_STATIC: return "lightmap_global_static";
    case ExternalRenderPassType::LIGHTMAP_GLOBAL_DYNAMIC: return "lightmap_global_dynamic";
    case ExternalRenderPassType::LIGHTMAP_BLACK_GLOBAL_STATIC: return "lightmap_black_global_static";
    case ExternalRenderPassType::LIGHTMAP_BLACK_LOCAL_INSTANCES: return "lightmap_black_local_instances";
    case ExternalRenderPassType::LIGHTMAP_BLACK_MOVABLES: return "lightmap_black_movables";
    case ExternalRenderPassType::LIGHTMAP_BLACK_NODE: return "lightmap_black_node";
    case ExternalRenderPassType::LIGHTMAP_BLACK_GLOBAL_AND_LOCAL: return "lightmap_black_global_and_local";
    case ExternalRenderPassType::LIGHTMAP_BLOBS: return "lightmap_blobs";
    case ExternalRenderPassType::STANDARD_FOREGROUND: return "standard|foreground";
    case ExternalRenderPassType::STANDARD_BACKGROUND: return "standard|background";
    default:
        throw std::runtime_error("Unknown render pass type");
    }
}

std::ostream& VA::operator << (std::ostream& ostr, ExternalRenderPassType pass) {
    ostr << external_render_pass_type_to_string(pass);
    return ostr;
}
