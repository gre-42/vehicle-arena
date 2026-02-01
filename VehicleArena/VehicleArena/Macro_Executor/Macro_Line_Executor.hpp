// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Macro_Executor/Boolean_Expression.hpp>
#include <VehicleArena/Macro_Executor/Json_Macro_Arguments.hpp>
#include <VehicleArena/Regex/Misc.hpp>
#include <filesystem>
#include <functional>
#include <list>
#include <map>
#include <string>

namespace VA {

class MacroRecorder;
class SubstitutionMap;
class NotifyingJsonMacroArguments;
class JsonMacroArgumentsObserverToken;
class AssetReferences;
class FPath;
class JsonView;
class JsonMacroArgumentsAndLock;
class WritableJsonMacroArgumentsAndLock;

class MacroLineExecutor {
    friend MacroRecorder;
public:
    typedef std::function<bool(
        const std::string& context,
        const MacroLineExecutor& macro_line_executor,
        const std::string& name,
        const JsonMacroArguments& arguments,
        JsonMacroArguments* local_json_macro_arguments)> JsonUserFunction;
    MacroLineExecutor(
        MacroRecorder& macro_recorder,
        std::string script_filename,
        const std::list<std::string>* search_path,
        JsonUserFunction json_user_function,
        std::string context,
        nlohmann::json block_arguments,
        NotifyingJsonMacroArguments& global_json_macro_arguments,
        const AssetReferences& asset_references,
        bool verbose);
    MacroLineExecutor changed_script_filename(
        std::string script_filename) const;
    MacroLineExecutor inserted_block_arguments(
        nlohmann::json block_arguments) const;
    MacroLineExecutor changed_context(
        std::string context,
        nlohmann::json block_arguments) const;
    MacroLineExecutor changed_script_filename_and_context(
        std::string script_filename,
        std::string context,
        nlohmann::json block_arguments) const;
    void operator () (
        const nlohmann::json& j,
        JsonMacroArguments* local_json_macro_arguments) const;
    nlohmann::json eval(const std::string& expression) const;
    nlohmann::json eval(const std::string& expression, const JsonView& variables) const;
    template <class T>
    T eval(const std::string& expression) const;
    template <class T>
    T eval(const std::string& expression, const JsonView& variables) const;
    nlohmann::json at(const std::string& key) const;
    template <class T>
    T at(const std::string& key) const;
    JsonMacroArgumentsAndLock json_macro_arguments() const;
    WritableJsonMacroArgumentsAndLock writable_json_macro_arguments();
    bool eval(const BooleanExpression& expression) const;
    bool eval(const BooleanExpression& expression, const JsonView& variables) const;
    bool eval_boolean_expression(const nlohmann::json& j) const;
    bool eval_boolean_expression(const nlohmann::json& j, const JsonView& variables) const;
    JsonMacroArgumentsObserverToken add_observer(std::function<void()> func);
    JsonMacroArgumentsObserverToken add_finalizer(std::function<void()> func);
    JsonView block_arguments() const;
private:
    MacroRecorder& macro_recorder_;
    std::string script_filename_;
    const std::list<std::string>* search_path_;
    JsonUserFunction json_user_function_;
    std::string context_;
    nlohmann::json block_arguments_;
    NotifyingJsonMacroArguments& global_json_macro_arguments_;
    const AssetReferences& asset_references_;
    bool verbose_;
};

}
