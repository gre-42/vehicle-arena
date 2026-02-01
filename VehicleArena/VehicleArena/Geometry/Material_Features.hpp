// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Hashing/Std_Hash.hpp>
#include <cstddef>
#include <unordered_map>
#include <vector>

namespace VA {

class ITextureHandle;
struct BlendMapTexture;
struct ColormapWithModifiers;
struct Material;
template <typename TData, size_t... tshape>
class FixedArray;

struct BlendMapTextureAndId {
    const BlendMapTexture* ops;
    const ITextureHandle* tex_color;
    const ITextureHandle* tex_specular;
    const ITextureHandle* tex_normal;
    size_t id_color;
    size_t id_specular;
    size_t id_normal;
    const BlendMapTexture& operator * () const {
        return *ops;
    }
    const BlendMapTexture* operator -> () const {
        return ops;
    }
};

class ColormapPtr {
public:
    inline ColormapPtr(const ColormapWithModifiers& cm): cm_{&cm} {}
    inline const ColormapWithModifiers& operator * () const {
        return *cm_;
    }
    inline const ColormapWithModifiers* operator -> () const {
        return cm_;
    }
    bool operator == (const ColormapPtr& other) const;
private:
    const ColormapWithModifiers* cm_;
};

bool has_normalmap(const std::vector<BlendMapTexture>& textures_color);
bool fragments_depend_on_distance(
    const FixedArray<float, 2>& fog_distances,
    const FixedArray<float, 4>& alpha_distances,
    const std::vector<BlendMapTextureAndId>& textures_color);
bool fragments_depend_on_normal(const std::vector<BlendMapTexture>& textures_color);
bool fragments_depend_on_normal(const std::vector<BlendMapTextureAndId>& textures_color);
bool has_horizontal_detailmap(const std::vector<BlendMapTextureAndId>& textures);

}

template <>
struct std::hash<VA::ColormapPtr>
{
    std::size_t operator() (const VA::ColormapPtr& k) const;
};
