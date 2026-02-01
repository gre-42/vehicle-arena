// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <string>

namespace VA {

enum class ColorMode {
    NONE = 0,
    UNDEFINED = 1 << 0,
    GRAYSCALE = 1 << 1,
    RGB = 1 << 2,
    RGBA = 1 << 3,
    AGR_NORMAL = 1 << 4
};

inline ColorMode operator & (ColorMode a, ColorMode b) {
    return (ColorMode)((int)a & (int)b);
}

inline ColorMode operator | (ColorMode a, ColorMode b) {
    return (ColorMode)((int)a | (int)b);
}

inline ColorMode& operator |= (ColorMode& a, ColorMode b) {
    (int&)a |= (int)b;
    return a;
}

inline bool any(ColorMode a) {
    return a != ColorMode::NONE;
}

size_t max(ColorMode mode);
ColorMode color_mode_from_channels(size_t nchannels);
ColorMode color_mode_from_string(const std::string& str);
std::string color_mode_to_string(const ColorMode& mode);

}
