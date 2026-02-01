// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Altitude_Azimuth.hpp"
#include <VehicleArena/Math/Math.hpp>
#include <VehicleArena/Physics/Units.hpp>

using namespace VA;

void VA::hadec2altaz(double ha, double dec, double lat, double& alt, double& az) {
    auto sh = std::sin(ha);
    auto ch = std::cos(ha);
    auto sd = std::sin(dec);
    auto cd = std::cos(dec);
    auto sl = std::sin(lat);
    auto cl = std::cos(lat);

    auto x = - ch * cd * sl + sd * cl;
    auto y = - sh * cd;
    auto z = ch * cd * cl + sd * sl;
    auto r = std::sqrt(squared(x) + squared(y));

    // Now get Alt, Az
    az = std::atan2(y, x);
    alt = std::atan2(z, r);

    // Correct for negative AZ
    if (az < 0) {
        az += 360. * degrees;
    }
}
