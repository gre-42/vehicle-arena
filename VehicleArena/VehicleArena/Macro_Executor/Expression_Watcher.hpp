// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Macro_Executor/Macro_Line_Executor.hpp>
#include <VehicleArena/Macro_Executor/Notifying_Json_Macro_Arguments.hpp>
#include <VehicleArena/Threads/Fast_Mutex.hpp>

namespace VA {

class ExpressionWatcher {
public:
    inline ExpressionWatcher(MacroLineExecutor mle)
        : mle_{ std::move(mle) }
        , result_may_have_changed_{ true }
        , ot_{ mle_.add_observer([this](){
            std::scoped_lock lock{ mutex_ };
            result_may_have_changed_ = true;
            }) }
    {}
    ~ExpressionWatcher() = default;
    inline bool result_may_have_changed() const {
        std::scoped_lock lock{ mutex_ };
        auto res = result_may_have_changed_;
        result_may_have_changed_ = false;
        return res;
    }
    inline JsonMacroArgumentsObserverToken add_observer(std::function<void()> func) {
        return mle_.add_observer(std::move(func));
    }
    inline nlohmann::json eval(const std::string& expression) const {
        return mle_.eval(expression);
    }
    inline nlohmann::json eval(const std::string& expression, const JsonView& variables) const {
        return mle_.eval(expression, variables);
    }
    template <class T>
    T eval(const std::string& expression) const {
        return mle_.eval<T>(expression);
    }
    template <class T>
    T eval(const std::string& expression, const JsonView& variables) const {
        return mle_.eval<T>(expression, variables);
    }
    bool eval(const std::vector<std::vector<std::string>>& expression) const {
        return mle_.eval(expression);
    }
    bool eval(const std::vector<std::vector<std::string>>& expression, const JsonView& variables) const {
        return mle_.eval(expression, variables);
    }
    inline void execute(
        const nlohmann::json& j,
        JsonMacroArguments* local_json_macro_arguments) const
    {
        mle_(j, local_json_macro_arguments);
    }
private:
    MacroLineExecutor mle_;
    mutable bool result_may_have_changed_;
    JsonMacroArgumentsObserverToken ot_;
    mutable FastMutex mutex_;
};

}
