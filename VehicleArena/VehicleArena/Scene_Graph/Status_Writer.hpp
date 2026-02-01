// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Hashing/Variable_And_Hash.hpp>
#include <iosfwd>
#include <string>
#include <vector>

namespace VA {

struct StaticWorld;

enum class StatusComponents {
    NONE = 0,
    TIME = 1 << 0,
    POSITION = 1 << 1,
    SPEED = 1 << 2,
    HEALTH = 1 << 3,
    DIAMETER = 1 << 4,
    ENERGY = 1 << 5,
    DRIVER_NAME = 1 << 6,
    ANGULAR_VELOCITY = 1 << 7,
    WHEEL_ANGULAR_VELOCITY = 1 << 8,
    ABS_ANGULAR_VELOCITY = 1 << 9
};

inline bool operator & (StatusComponents a, StatusComponents b) {
    return ((unsigned int)a & (unsigned int)b) != 0;
}

inline bool operator |= (StatusComponents& a, StatusComponents b) {
    return (unsigned int&)a |= (unsigned int)b;
}

class StatusWriter {
public:
    virtual void write_status(
        std::ostream& ostr,
        StatusComponents status_components,
        const StaticWorld& world) const = 0;
    virtual float get_value(StatusComponents status_components) const = 0;
    virtual StatusWriter& child_status_writer(const std::vector<VariableAndHash<std::string>>& name) = 0;
};

StatusComponents status_components_from_string(const std::string& s);

}
