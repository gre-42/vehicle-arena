// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Race_Identifier.hpp"
#include <sstream>
#include <stdexcept>

using namespace VA;

void check_dirname_part(const std::string& part) {
    for (char c : part) {
        if ((c >= '0') && (c <= '9')) {
            continue;
        }
        if ((c >= 'A') && (c <= 'Z')) {
            continue;
        }
        if ((c >= 'a') && (c <= 'z')) {
            continue;
        }
        if ((c == '_') || (c == '-') || (c == '.')) {
            continue;
        }
        throw std::runtime_error("Invalid charactar in dirname part (must be 0-9, A-Z, a-z, _ or -): \"" + part + '"');
    }
}

std::string RaceIdentifier::dirname() const {
    if (level.empty()) {
        throw std::runtime_error("Empty level name");
    }
    if (session.empty()) {
        throw std::runtime_error("Empty session name");
    }
    check_dirname_part(level);
    check_dirname_part(time_of_day);
    check_dirname_part(restrictions);
    check_dirname_part(session);
    std::stringstream sstr;
    sstr << level << '.' << session;
    if (time_of_day != "day") {
        sstr << '.' << time_of_day;
    }
    if (!restrictions.empty()) {
        sstr << '.' << restrictions;
    }
    if (laps != 0) {
        sstr << "." << laps;
    }
    if (milliseconds != 0) {
        sstr << "." << milliseconds;
    }
    return sstr.str();
}
