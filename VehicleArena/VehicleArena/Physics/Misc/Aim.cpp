// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Aim.hpp"
#include <VehicleArena/Array/Fixed_Array.hpp>
#include <VehicleArena/Math/Optimize/Newton_1D.hpp>
#include <VehicleArena/Math/Pi.hpp>
#include <cmath>

using namespace VA;

// solve([x=cos(a)*v*t, y=sin(a)*v*t-1/2*g*t^2], [x, y, a, t, g]);
// https://en.wikipedia.org/wiki/Rifleman%27s_rule
// https://en.wikipedia.org/wiki/Trigonometric_functions
// solve(s=h*(1-tan(d)*tan(a))/cos(a), d)
// sin(d)=-(cos(d)*(cos(a)*s-h))/(tan(a)*h)
// sec=1/cos
// tan=sin/cos

// y / x = c;
// solve(c=tan(a)*v*t-1/2*g*t^2/cos(a)/v/t, a);
// solve(tan(a)*v*t-1/2*g*t^2/cos(a)/v/t+c, a);
// tan(a)=(2*c*v+g*t)/(2*t*v^2)
Aim::Aim(
    const FixedArray<ScenePos, 3>& gun_pos,
    const FixedArray<ScenePos, 3>& target_pos,
    ScenePos bullet_start_offset,
    ScenePos velocity,
    ScenePos gravity,
    ScenePos eps,
    size_t niterations)
{
    // Maxima code
    // t(a) := (x - cos(a) * bullet_start_offset) / (cos(a) * velocity);
    // yt(a) := sin(a) * bullet_start_offset + sin(a) * velocity * t(a) - gravity / 2 * t(a)^2;
    // diff(yt(a) - y, a);
    auto x = std::sqrt(
        squared(gun_pos(0) - target_pos(0)) +
        squared(gun_pos(2) - target_pos(2)));
    auto y = target_pos(1) - gun_pos(1);
    angle0 = std::atan2(y, x);

    auto t = [x, bullet_start_offset, velocity](ScenePos a) {
        auto ca = std::cos(a);
        return (x - ca * bullet_start_offset) / (ca * velocity);
    };
    auto ft = [y, bullet_start_offset, velocity, gravity](ScenePos a, ScenePos ta) {
        auto sa = std::sin(a);
        auto yt = sa * bullet_start_offset + sa * velocity * ta - gravity / 2 * squared(ta);
        return yt - y;
    };
    auto f = [&t, &ft](ScenePos a) {
        return ft(a, t(a));
    };
    auto df = [x, bullet_start_offset, velocity, gravity](ScenePos a) {
        auto ca = std::cos(a);
        auto sa = std::sin(a);
        return
            -(sa * gravity * squared(x - ca * bullet_start_offset)) /
            (cubed(ca) * squared(velocity))
            - (sa * bullet_start_offset * gravity * (x - ca * bullet_start_offset)) /
            squared(ca * velocity)
            + (squared(sa) * (x - ca * bullet_start_offset)) /
            squared(ca) + x + (squared(sa) * bullet_start_offset) / ca;
        };
    angle = NAN;
    for (ScenePos a = angle0; a < ScenePos(M_PI / 2); a += ScenePos(0.2)) {
        if (f(a) > 0) {
            angle = newton_1d(f, df, a, eps, niterations, false);
            break;
        }
    }
    if (std::isnan(angle)) {
        aim_offset = NAN;
        time = NAN;
    } else {
        aim_offset = (std::tan(angle) - std::tan(angle0)) * x;
        time = t(angle);
    }
}
