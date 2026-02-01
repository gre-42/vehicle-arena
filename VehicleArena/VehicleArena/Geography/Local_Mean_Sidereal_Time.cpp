// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Local_Mean_Sidereal_Time.hpp"
#include <VehicleArena/Geography/Idl_Mod.hpp>
#include <VehicleArena/Math/Math.hpp>
#include <VehicleArena/Physics/Units.hpp>

using namespace VA;

double VA::ct2lst(double jddays, double lon) {
    // Useful constants, see Meeus, p.84
    const double c[] = {
        280.46061837, 360.98564736629,
        0.000387933, 38710000.0};
    auto time = jddays / 36525.0;

    // Compute GST in seconds.
    auto theta = (c[0] + (c[1] * jddays) + (c[2] - time / c[3]) * squared(time)) * degrees;

    // Compute LST in hours.
    auto lst = (theta + lon) / (15.0 * degrees);
    if (lst < 0.0) {
        lst = 24.0 + idl_mod(lst, 24.);
    }

    // Local sidereal time in hours (0. to 24.)
    lst = idl_mod(lst, 24.0);

    return lst;
}
