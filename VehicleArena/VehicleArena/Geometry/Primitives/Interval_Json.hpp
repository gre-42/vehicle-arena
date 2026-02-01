// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include "Interval.hpp"
#include <VehicleArena/Json/Json_View.hpp>
#include <VehicleArena/Misc/Argument_List.hpp>
#include <stdexcept>
#include <vector>

namespace VA {

namespace IntervalArgs {
BEGIN_ARGUMENT_LIST;
DECLARE_ARGUMENT(min);
DECLARE_ARGUMENT(max);
}

template <class T>
void from_json(const nlohmann::json& j, Interval<T>& i) {
    if (j.is_array()) {
        auto vd = j.get<std::vector<T>>();
        if (vd.size() != 2) {
            throw std::runtime_error("Interval array does not have length 2");
        }
        i = { vd[0], vd[1] };
    } else {
        JsonView jv{ j };
        jv.validate(IntervalArgs::options);
        i.min = jv.at<T>(IntervalArgs::min);
        i.max = jv.at<T>(IntervalArgs::max);
    }
}

}
