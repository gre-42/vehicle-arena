// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Respawn_Config.hpp"
#include <VehicleArena/Json/Json_View.hpp>
#include <VehicleArena/Json/Misc.hpp>
#include <VehicleArena/Misc/Argument_List.hpp>

using namespace VA;

namespace KnownArgs {
BEGIN_ARGUMENT_LIST;
DECLARE_ARGUMENT(max_respawn_distance);
DECLARE_ARGUMENT(vehicle_length);
DECLARE_ARGUMENT(max_horizontal_angle);
DECLARE_ARGUMENT(max_vertical_angle);
}

void VA::from_json(const nlohmann::json& j, RespawnConfig& cfg) {
    JsonView jv{ j };
    jv.validate(KnownArgs::options);
    cfg.max_respawn_distance = jv.at<ScenePos>(KnownArgs::max_respawn_distance) * meters;
    cfg.vehicle_length = jv.at<ScenePos>(KnownArgs::vehicle_length) * meters;
    cfg.max_horizontal_angle = jv.at<float>(KnownArgs::max_horizontal_angle) * degrees;
    cfg.max_vertical_angle = jv.at<float>(KnownArgs::max_vertical_angle) * degrees;
}
