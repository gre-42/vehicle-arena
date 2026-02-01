// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Texture_Target.hpp"
#include <ostream>
#include <stdexcept>
#include <string>

using namespace VA;

std::ostream& VA::operator << (std::ostream& ostr, TextureTarget target) {
    switch (target) {
    case TextureTarget::NONE:
        return ostr << "none";
    case TextureTarget::TEXTURE_2D:
        return ostr << "texture_2d";
    case TextureTarget::TEXTURE_2D_ARRAY:
        return ostr << "texture_2d_array";
    case TextureTarget::TEXTURE_3D:
        return ostr << "texture_3d";
    case TextureTarget::TEXTURE_CUBE_MAP:
        return ostr << "texture_cube_map";
    case TextureTarget::ONE_LAYER_MASK:
        return ostr << "one_layer_mask";
    }
    throw std::runtime_error("Unknown texture type: " + std::to_string((int)target));
}
