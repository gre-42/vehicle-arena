// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Physics/Actuators/Engine_Power_Delta_Intent.hpp>

namespace VA {

class RigidBodyDeltaEngine {
    friend std::ostream& operator << (std::ostream& ostr, const RigidBodyDeltaEngine& engine);

    RigidBodyDeltaEngine(const RigidBodyDeltaEngine&) = delete;
    RigidBodyDeltaEngine& operator = (const RigidBodyDeltaEngine&) = delete;
public:
    RigidBodyDeltaEngine();
    ~RigidBodyDeltaEngine();

    inline void set_surface_power(const EnginePowerDeltaIntent& engine_power_delta_intent) {
        engine_power_delta_intent_ = engine_power_delta_intent;
    }

    inline const EnginePowerDeltaIntent& engine_power_delta_intent() const {
        return engine_power_delta_intent_;
    }
private:
    EnginePowerDeltaIntent engine_power_delta_intent_;
};

std::ostream& operator << (std::ostream& ostr, const RigidBodyDeltaEngine& delta_engine);

}
