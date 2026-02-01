// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Interpolation_Mode.hpp"
#include <map>
#include <stdexcept>

using namespace std::string_view_literals;
using namespace VA;

InterpolationMode VA::interpolation_mode_from_string(std::string_view str) {
    static const std::map<std::string_view, InterpolationMode> m{
        { "nearest"sv,  InterpolationMode::NEAREST},
        { "linear"sv,  InterpolationMode::LINEAR}
    };
    auto it = m.find(str);
    if (it == m.end()) {
        throw std::runtime_error("Unknown interpolation mode: \"" + std::string{ str } + '"');
    }
    return it->second;
}

std::string_view VA::interpolation_mode_to_string(InterpolationMode mode) {
    switch (mode) {
    case InterpolationMode::NEAREST:
        return "nearest"sv;
    case InterpolationMode::LINEAR:
        return "linear"sv;
    }
    throw std::runtime_error("Unknown interpolation mode: " + std::to_string((int)mode));
}
