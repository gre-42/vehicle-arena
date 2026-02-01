// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <string>

namespace VA {

enum class ScoreBoardConfiguration {
    NONE            = 0,
    USER            = (1 << 0),
    PLAYER          = (1 << 1),
    TEAM            = (1 << 2),
    BEST_LAP_TIME   = (1 << 3),
    CAR_HP          = (1 << 4),
    HISTORY         = (1 << 5),
    NWINS           = (1 << 6),
    NKILLS          = (1 << 7),
    LAPS            = (1 << 8),
    RANK            = (1 << 9),
    RACE_TIME       = (1 << 10)
};

inline ScoreBoardConfiguration operator & (ScoreBoardConfiguration a, ScoreBoardConfiguration b) {
    return (ScoreBoardConfiguration)((int)a & (int)b);
}

inline ScoreBoardConfiguration operator | (ScoreBoardConfiguration a, ScoreBoardConfiguration b) {
    return (ScoreBoardConfiguration)((unsigned int)a | (unsigned int)b);
}

inline ScoreBoardConfiguration& operator |= (ScoreBoardConfiguration& a, ScoreBoardConfiguration b) {
    a = a | b;
    return a;
}

inline bool any(ScoreBoardConfiguration c) {
    return c != ScoreBoardConfiguration::NONE;
}

ScoreBoardConfiguration score_board_configuration_from_string(const std::string& s);

}
