// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Blend_Mode.hpp"
#include <map>
#include <stdexcept>

using namespace VA;

BlendMode VA::blend_mode_from_string(const std::string& str) {
    static const std::map<std::string, BlendMode> m{
        {"off", BlendMode::OFF},
        {"invisible", BlendMode::INVISIBLE},
        {"binary_05", BlendMode::BINARY_05},
        {"binary_08", BlendMode::BINARY_08},
        {"semi_continuous_02", BlendMode::SEMI_CONTINUOUS_02},
        {"semi_continuous_08", BlendMode::SEMI_CONTINUOUS_08},
        {"continuous", BlendMode::CONTINUOUS},
        {"binary_05_add", BlendMode::BINARY_05_ADD},
        {"continuous_add", BlendMode::CONTINUOUS_ADD}
    };
    auto it = m.find(str);
    if (it == m.end()) {
        throw std::runtime_error("Unknown blend mode: \"" + str + '"');
    }
    return it->second;
}
