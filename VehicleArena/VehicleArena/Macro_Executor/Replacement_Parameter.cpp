// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Replacement_Parameter.hpp"
#include <VehicleArena/Json/Json_View.hpp>
#include <VehicleArena/Misc/Argument_List.hpp>
#include <VehicleArena/Os/Os.hpp>
#include <filesystem>
#include <fstream>
#include <stdexcept>

namespace fs = std::filesystem;

using namespace VA;

namespace KnownRequired {
BEGIN_ARGUMENT_LIST;
DECLARE_ARGUMENT(fixed);
DECLARE_ARGUMENT(dynamic);
DECLARE_ARGUMENT(focus_mask);
}

namespace KnownArgs {
BEGIN_ARGUMENT_LIST;
DECLARE_ARGUMENT(id);
DECLARE_ARGUMENT(title);
DECLARE_ARGUMENT(database);
DECLARE_ARGUMENT(required);
DECLARE_ARGUMENT(on_init);
DECLARE_ARGUMENT(on_before_select);
DECLARE_ARGUMENT(on_execute);
}

ReplacementParameterAndFilename ReplacementParameterAndFilename::from_json(const std::string& filename) {
    try {
        nlohmann::json j;
        auto ifs_p = create_ifstream(filename);
        auto& ifs = *ifs_p;
        if (ifs.fail()) {
            throw std::runtime_error("Could not open replacement parameter entry file \"" + filename + '"');
        }
        ifs >> j;
        if (!ifs.eof() && ifs.fail()) {
            throw std::runtime_error("Error reading from file: \"" + filename + '"');
        }
        return ReplacementParameterAndFilename{
            .rp = j.get<ReplacementParameter>(),
            .filename = filename};
    } catch (const nlohmann::json::exception& e) {
        throw std::runtime_error("Error loading file \"" + filename + "\": " + e.what());
    }
}

void VA::from_json(const nlohmann::json& j, ReplacementParameterRequired& rp) {
    JsonView jv{ j };
    jv.validate(KnownRequired::options);
    expression_from_json(jv.at(KnownRequired::fixed), rp.fixed);
    expression_from_json(jv.at(KnownRequired::dynamic), rp.dynamic);
    rp.focus_mask = jv.contains(KnownRequired::focus_mask)
        ? focus_from_string(jv.at(KnownRequired::focus_mask).get<std::string>())
        : Focus::ALWAYS;
}

void VA::from_json(const nlohmann::json& j, ReplacementParameter& rp) {
    JsonView jv{ j };
    jv.validate(KnownArgs::options);
    jv.at(KnownArgs::id).get_to(rp.id);
    jv.at(KnownArgs::title).get_to(rp.title);
    if (jv.contains(KnownArgs::required)) {
        jv.at(KnownArgs::required).get_to(rp.required);
    }
    if (jv.contains(KnownArgs::database)) {
        rp.database.merge(JsonMacroArguments{jv.at(KnownArgs::database)});
    }
    if (jv.contains(KnownArgs::on_init)) {
        jv.at(KnownArgs::on_init).get_to(rp.on_init);
    }
    if (jv.contains(KnownArgs::on_before_select)) {
        jv.at(KnownArgs::on_before_select).get_to(rp.on_before_select);
    }
    if (jv.contains(KnownArgs::on_execute)) {
        jv.at(KnownArgs::on_execute).get_to(rp.on_execute);
    }
}
