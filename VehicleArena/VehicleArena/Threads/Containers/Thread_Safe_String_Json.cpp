// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Thread_Safe_String_Json.hpp"
#include <VehicleArena/Json/Base.hpp>

using namespace VA;

void VA::from_json(const nlohmann::json& j, ThreadSafeString& t) {
    std::string s;
    nlohmann::detail::from_json(j, s);
    t = s;
}

void VA::to_json(nlohmann::json& j, const ThreadSafeString& t) {
    nlohmann::detail::to_json(j, (std::string)t);
}
