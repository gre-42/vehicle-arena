// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Shock_Absorber.hpp"
#include <stdexcept>

using namespace VA;

ShockAbsorber::ShockAbsorber(float Ks, float Ka)
: Ks_{Ks},
  Ka_{Ka},
  position_{0},
  F_{0}
{}

void ShockAbsorber::integrate_force(float F) {
    F_ += F;
}

void ShockAbsorber::advance_time(float dt, IntegrationMode integration_mode) {
    // Solve: K_spring * x + K_absorber * dx / dt = F
    // Solve: K_spring * x{0,1} + K_absorber * (x1 - x0) / dt = F

    // Implicit: solve(s * x1 + a * (x1 - x0) / t = F, x1);
    // Explicit: solve(s * x0 + a * dx / t = F, dx);
    // Semiimpl: solve(s * (x1 + x0) / 2 + a * (x1 - x0) / t = F, x1);

    switch(integration_mode) {
        case IntegrationMode::IMPLICIT:
            position_ = (F_ + Ka_ / dt * position_) / (Ks_ + Ka_ / dt);
            break;
        case IntegrationMode::EXPLICIT:
            position_ += dt / Ka_ * (F_ - Ks_ * position_);
            break;
        case IntegrationMode::SEMI_IMPLICIT:
            position_ = -((Ks_ * dt - 2 * Ka_) * position_ - 2 * F_ * dt) / (Ks_ * dt + 2 * Ka_);
            break;
        default:
            throw std::runtime_error("Unknown integration mode");
    }
    F_ = 0;
}
