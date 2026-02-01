// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Misc/Argument_List.hpp>
#include <set>

namespace VA {

namespace MacroKeys {
BEGIN_ARGUMENT_LIST;
// One of
DECLARE_ARGUMENT(call);
DECLARE_ARGUMENT(declare_macro);
DECLARE_ARGUMENT(playback);
DECLARE_ARGUMENT(execute);
DECLARE_ARGUMENT(include);
DECLARE_ARGUMENT(comment);
// Only macro
DECLARE_ARGUMENT(content);
// Macro and function
DECLARE_ARGUMENT(required);
DECLARE_ARGUMENT(exclude);
DECLARE_ARGUMENT(context);
DECLARE_ARGUMENT(arguments);
DECLARE_ARGUMENT(let);
DECLARE_ARGUMENT(with);
DECLARE_ARGUMENT(without);
}

namespace UserKeys {
BEGIN_ARGUMENT_LIST;
DECLARE_ARGUMENT(title);
DECLARE_ARGUMENT(charset);
}

static std::set<std::string_view> unexpanded_keys =
{
    UserKeys::title,
    UserKeys::charset,
    MacroKeys::required,
    MacroKeys::exclude,
    MacroKeys::arguments,
    MacroKeys::content,
    MacroKeys::comment,
    MacroKeys::context
};

}
