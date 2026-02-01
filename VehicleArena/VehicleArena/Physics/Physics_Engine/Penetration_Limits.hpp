// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once

namespace VA {

struct PenetrationLimits {
    constexpr static const float FAC_R = 1 / 4.f;
    constexpr static const float FAC_T = 1 - FAC_R;
    constexpr static const float FAC_PAIR = 1 / 2.f;
    PenetrationLimits(float dt, float max_penetration)
    {
        float vmax_total = max_penetration / dt;
        vmax_rotation = vmax_total * FAC_R * FAC_PAIR;
        vmax_translation = vmax_total * FAC_T * FAC_PAIR;
    }
    inline float wmax(float radius) const {
        return vmax_rotation / radius;
    }
    float vmax_rotation;
    float vmax_translation;
};

}
