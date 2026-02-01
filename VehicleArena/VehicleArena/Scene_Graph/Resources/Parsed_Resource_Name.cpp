// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Parsed_Resource_Name.hpp"
#include <VehicleArena/Array/Fixed_Array.hpp>
#include <VehicleArena/Geometry/Billboard_Id.hpp>
#include <VehicleArena/Geometry/Material/Aggregate_Mode.hpp>
#include <VehicleArena/Physics/Units.hpp>
#include <VehicleArena/Regex/Regex_Select.hpp>
#include <VehicleArena/Scene_Graph/Descriptors/Resource_Instance_Descriptor.hpp>
#include <VehicleArena/Scene_Graph/Resources/Scene_Node_Resources.hpp>
#include <stdexcept>

using namespace VA;

#define BEGIN_OPTIONS static size_t option_id = 1
#define DECLARE_OPTION(a) static const size_t a = option_id++

BEGIN_OPTIONS;
DECLARE_OPTION(NAME);
DECLARE_OPTION(BILLBOARD_ID);
DECLARE_OPTION(YANGLE);
DECLARE_OPTION(PROBABILITY);
DECLARE_OPTION(PROBABILITY1);
DECLARE_OPTION(MIN_BDRY);
DECLARE_OPTION(MAX_BDRY);
DECLARE_OPTION(HITBOX);

ParsedResourceName VA::parse_resource_name(
    const SceneNodeResources& resources,
    const std::string& name)
{
    static const DECLARE_REGEX(re,
        "^([^.(\\s]*)"
        "(?:\\.(\\d+))?"
        "(?:\\s*\\(yangle:([\\d+.e-]+)\\))?"
        "(?:\\s*\\(p:([\\d+.e-]+)\\))?"
        "(?:\\s*\\(p1:([\\d+.e-]+)\\))?"
        "(?:\\s*\\(min_bdry:([\\d+.e-]+)\\))?"
        "(?:\\s*\\(max_bdry:([\\d+.e-]+)\\))?"
        "(?:\\s*\\(hitbox:(\\w+)\\))?$");
    VA::re::cmatch match;
    if (!VA::re::regex_match(name, match, re)) {
        throw std::runtime_error("Could not parse: " + name);
    }
    ParsedResourceName result{
        .name = VariableAndHash{ match[NAME].str() },
        .billboard_id = match[BILLBOARD_ID].matched ? safe_stox<BillboardId>(match[BILLBOARD_ID].str()) : BILLBOARD_ID_NONE,
        .yangle = match[YANGLE].matched ? safe_stof(match[YANGLE].str()) * degrees : 0.f,
        .probability = match[PROBABILITY].matched ? safe_stof(match[PROBABILITY].str()) : 1.f,
        .probability1 = match[PROBABILITY1].matched ? safe_stof(match[PROBABILITY1].str()) : 1.f,
        .min_distance_to_bdry = match[MIN_BDRY].matched ? safe_stof(match[MIN_BDRY].str()) : 0.f,
        .max_distance_to_bdry = match[MAX_BDRY].matched ? safe_stof(match[MAX_BDRY].str()) : INFINITY,
        .aggregate_mode = resources.aggregate_mode(VariableAndHash<std::string>{match[NAME].str()}),
        .create_imposter = false,
        .max_imposter_texture_size = 0,
        .hitbox = VariableAndHash<std::string>{ match[HITBOX].str() },
        .supplies_cooldown = NAN};
    if (result.probability < 1e-7) {
        throw std::runtime_error("ResourceNameCycle: threshold too small");
    }
    if (result.probability > 1) {
        throw std::runtime_error("ResourceNameCycle: threshold too large");
    }
    return result;
}
