// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Color_Mode.hpp"
#include <map>
#include <stdexcept>

using namespace VA;

size_t VA::max(ColorMode mode) {
    if (any(mode & ColorMode::RGBA)) {
        return 4;
    }
    if (any(mode & ColorMode::RGB)) {
        return 3;
    }
    if (any(mode & ColorMode::GRAYSCALE)) {
        return 1;
    }
    throw std::runtime_error("Unknown color mode: " + std::to_string((int)mode));
}

ColorMode VA::color_mode_from_channels(size_t nchannels) {
    switch (nchannels) {
    case 4:
        return ColorMode::RGBA;
    case 3:
        return ColorMode::RGB;
    case 1:
        return ColorMode::GRAYSCALE;
    }
    throw std::runtime_error("Unsupported number of channels: " + std::to_string(nchannels));
}

ColorMode VA::color_mode_from_string(const std::string& str) {
    const std::map<std::string, ColorMode> m{
        {"grayscale", ColorMode::GRAYSCALE},
        {"rgb", ColorMode::RGB},
        {"rgba", ColorMode::RGBA}
    };
    auto it = m.find(str);
    if (it == m.end()) {
        throw std::runtime_error("Unknown color mode: \"" + str + '"');
    }
    return it->second;
}

std::string VA::color_mode_to_string(const ColorMode& mode) {
    switch (mode) {
    case ColorMode::UNDEFINED:
        return "undefined";
    case ColorMode::GRAYSCALE:
        return "grayscale";
    case ColorMode::RGB:
        return "rgb";
    case ColorMode::RGBA:
        return "rgba";
    case ColorMode::AGR_NORMAL:
        return "agr_normal";
    default:
        return "ColorMode(" + std::to_string((int)mode) + ')';
    }
}
