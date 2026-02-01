// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Billboard_Atlas_Instance_Json.hpp"
#include <VehicleArena/Geometry/Material/Billboard_Atlas_Instance.hpp>
#include <VehicleArena/Geometry/Material/Render_Pass.hpp>
#include <VehicleArena/Json/Json_View.hpp>
#include <VehicleArena/Misc/Argument_List.hpp>

namespace BB {
BEGIN_ARGUMENT_LIST;
DECLARE_ARGUMENT(uv_scale);
DECLARE_ARGUMENT(uv_offset);
DECLARE_ARGUMENT(vertex_scale);
DECLARE_ARGUMENT(layer);
DECLARE_ARGUMENT(alpha_distances);
DECLARE_ARGUMENT(max_center_distance);
DECLARE_ARGUMENT(occluder_pass);
}
    
using namespace VA;

void VA::from_json(const nlohmann::json& j, BillboardAtlasInstance& bb) {
    JsonView jv{ j };
    jv.validate(BB::options);
    j.at(BB::uv_scale).get_to(bb.uv_scale);
    j.at(BB::uv_offset).get_to(bb.uv_offset);
    j.at(BB::vertex_scale).get_to(bb.vertex_scale);
    bb.texture_layer = jv.at<uint8_t>(BB::layer, 0);
    j.at(BB::alpha_distances).get_to(bb.alpha_distances);
    bb.max_center_distance2 = squared(json_get<ScenePos>(j.at(BB::max_center_distance)));
    bb.occluder_pass = external_render_pass_type_from_string(j.at(BB::occluder_pass).get<std::string>());
}
