// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Macro_Executor/Boolean_Expression.hpp>
#include <VehicleArena/Macro_Executor/Focus.hpp>
#include <VehicleArena/Macro_Executor/Json_Macro_Arguments.hpp>

namespace VA {

enum class Focus;

struct ReplacementParameterRequired {
    BooleanExpression fixed;
    BooleanExpression dynamic;
    Focus focus_mask = Focus::ALWAYS;
};

void from_json(const nlohmann::json& j, ReplacementParameterRequired& rp);

struct ReplacementParameter {
    std::string id;
    std::string title;
    ReplacementParameterRequired required;
    JsonMacroArguments database;
    nlohmann::json on_init;
    nlohmann::json on_before_select;
    nlohmann::json on_execute;
};

void from_json(const nlohmann::json& j, ReplacementParameter& rp);

struct ReplacementParameterAndFilename {
    static ReplacementParameterAndFilename from_json(const std::string& filename);
    ReplacementParameter rp;
    std::string filename;
};

}
