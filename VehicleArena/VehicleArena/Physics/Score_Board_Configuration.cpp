// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Score_Board_Configuration.hpp"
#include <VehicleArena/Strings/String.hpp>
#include <map>
#include <stdexcept>

using namespace VA;

static ScoreBoardConfiguration single_score_board_configuration_from_string(const std::string& s) {
    static const std::map<std::string, ScoreBoardConfiguration> m{
        {"none", ScoreBoardConfiguration::NONE},
        {"user", ScoreBoardConfiguration::USER},
        {"player", ScoreBoardConfiguration::PLAYER},
        {"team", ScoreBoardConfiguration::TEAM},
        {"best_lap_time", ScoreBoardConfiguration::BEST_LAP_TIME},
        {"car_hp", ScoreBoardConfiguration::CAR_HP},
        {"history", ScoreBoardConfiguration::HISTORY},
        {"nwins", ScoreBoardConfiguration::NWINS},
        {"nkills", ScoreBoardConfiguration::NKILLS},
        {"laps", ScoreBoardConfiguration::LAPS},
        {"rank", ScoreBoardConfiguration::RANK},
        {"race_time", ScoreBoardConfiguration::RACE_TIME},
    };
    auto it = m.find(s);
    if (it == m.end()) {
        throw std::runtime_error("Unknown score board configuration: \"" + s + '"');
    }
    return it->second;
}

ScoreBoardConfiguration VA::score_board_configuration_from_string(const std::string& s) {
    static const DECLARE_REGEX(re, "\\|");
    ScoreBoardConfiguration result = ScoreBoardConfiguration::NONE;
    for (const auto& m : string_to_list(s, re)) {
        result |= single_score_board_configuration_from_string(m);
    }
    return result;
}
