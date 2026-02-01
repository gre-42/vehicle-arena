// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Aperiodic_Reference_Time.hpp"
#include <VehicleArena/Memory/Integral_To_Float.hpp>
#include <VehicleArena/Os/Os.hpp>
#include <stdexcept>

using namespace VA;

AperiodicReferenceTime::AperiodicReferenceTime(
    std::chrono::steady_clock::time_point reference,
    std::chrono::steady_clock::duration duration)
    : reference_{ reference }
    , duration_{ duration }
{}

float AperiodicReferenceTime::phase01(
    std::chrono::steady_clock::time_point time) const
{
    if (duration_.count() <= 0) {
        throw std::runtime_error("AperiodicReferenceTime::phase01 on object with duration <= 0");
    }
    auto num = integral_to_float<double>((time - reference_).count());
    auto denom = integral_to_float<double>(duration_.count());
    return (float)(num / denom);
}

bool AperiodicReferenceTime::ran_to_completion(std::chrono::steady_clock::time_point time) const {
    return (duration_.count() != 0) && (time > reference_ + duration_);
}
