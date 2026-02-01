// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Translators.hpp"
#include <VehicleArena/Hashing/Variable_And_Hash.hpp>
#include <VehicleArena/Macro_Executor/Asset_Group_And_Id.hpp>
#include <VehicleArena/Macro_Executor/Asset_Group_Replacement_Parameters.hpp>
#include <VehicleArena/Macro_Executor/Asset_References.hpp>
#include <VehicleArena/Macro_Executor/Notifying_Json_Macro_Arguments.hpp>
#include <VehicleArena/Macro_Executor/Replacement_Parameter.hpp>

using namespace VA;

Translators::Translators(
    const AssetReferences& asset_references,
    NotifyingJsonMacroArguments& globals)
    : asset_references_{ asset_references }
    , ot_{ globals.add_observer([this, &globals](){
        if (language_variable_->empty()) {
            return;
        }
        auto j = globals.json_macro_arguments();
        auto& j1 = (const JsonMacroArguments&)j;
        auto l = j1.try_at<std::string>(*language_variable_);
        if (!l.has_value()) {
            return;
        }
        auto lang = VariableAndHash{ *l };
        if (language_ != lang) {
            std::scoped_lock lock{ mutex_ };
            language_ = lang;
            cached_dictionary_.clear();
        }
    }) }
{}

void Translators::set_language_variable(VariableAndHash<std::string> var) {
    std::scoped_lock lock{ mutex_ };
    if (!language_variable_->empty()) {
        throw std::runtime_error("Language variable already set");
    }
    language_variable_ = std::move(var);
}

std::string Translators::translate(
    const VariableAndHash<AssetGroupAndId>& gid,
    const VariableAndHash<std::string>& word) const
{
    std::scoped_lock lock{ mutex_ };
    if (language_->empty()) {
        return "??" + *word + "??";
    }
    auto& dict = cached_dictionary_[gid];
    auto it = dict.find(word);
    if (it == dict.end()) {
        const auto& j = asset_references_[*gid->group].at(*gid->id).rp.database;
        return JsonView{JsonView{j}.at(*word)}.at<std::string>(*language_);
    }
    return it->second;
}
