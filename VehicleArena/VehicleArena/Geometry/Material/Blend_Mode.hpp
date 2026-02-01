// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <string>

namespace VA {

/**
 * Explicit numbers to facilitate sorting for rendering.
 */
enum class BlendMode {
    INVISIBLE_MASK          = (1 << 0),
    THRESHOLD_02_MASK       = (1 << 1),
    THRESHOLD_05_MASK       = (1 << 2),
    THRESHOLD_08_MASK       = (1 << 3),
    BINARY_MASK             = (1 << 4),
    SEMI_CONTINUOUS_MASK    = (1 << 5),
    CONTINUOUS_MASK         = (1 << 6),
    ADD_MASK                = (1 << 7),

    OFF                     = 0,
    INVISIBLE               = INVISIBLE_MASK,
    BINARY_05               = THRESHOLD_05_MASK | BINARY_MASK,
    BINARY_08               = THRESHOLD_08_MASK | BINARY_MASK,
    SEMI_CONTINUOUS_02      = THRESHOLD_02_MASK | SEMI_CONTINUOUS_MASK,
    SEMI_CONTINUOUS_08      = THRESHOLD_08_MASK | SEMI_CONTINUOUS_MASK,
    CONTINUOUS              = CONTINUOUS_MASK,
    ANY_CONTINUOUS          = SEMI_CONTINUOUS_MASK | CONTINUOUS_MASK,
    BINARY_05_ADD           = THRESHOLD_05_MASK | BINARY_MASK | ADD_MASK,
    CONTINUOUS_ADD          = CONTINUOUS_MASK | ADD_MASK
};

inline BlendMode operator & (BlendMode a, BlendMode b) {
    return (BlendMode)((int)a & (int)b);
}

inline bool any(BlendMode a) {
    return a != BlendMode::OFF;
}

BlendMode blend_mode_from_string(const std::string& str);

}
