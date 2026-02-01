// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Array/Fixed_Array.hpp>
#include <VehicleArena/Memory/Integral_Cast.hpp>
#include <cstddef>
#include <cstdint>
#include <type_traits>

namespace VA {

enum class MipmapMode;
enum class ColorMode;
enum class WrapMode;
enum class InterpolationMode;
enum class TextureTarget;

class ITextureHandle {
public:
    virtual ~ITextureHandle() = default;
    template <class T>
    inline T handle() const {
        if constexpr (std::is_same_v<T, uint32_t>) {
            return handle32();
        } else if constexpr (std::is_same_v<T, uint64_t>) {
            return handle64();
        } else {
            throw std::runtime_error("Unsupported texture handle type");
        }
    }
    template <class T>
    inline T& handle() {
        if constexpr (std::is_same_v<T, uint32_t>) {
            return handle32();
        } else if constexpr (std::is_same_v<T, uint64_t>) {
            return handle64();
        } else {
            throw std::runtime_error("Unsupported texture handle type");
        }
    }
    virtual uint32_t handle32() const = 0;
    virtual uint64_t handle64() const = 0;
    virtual uint32_t& handle32() = 0;
    virtual uint64_t& handle64() = 0;
    virtual bool texture_is_loaded_and_try_preload() = 0;
    virtual TextureTarget target() const = 0;
    virtual ColorMode color_mode() const = 0;
    virtual MipmapMode mipmap_mode() const = 0;
    virtual InterpolationMode magnifying_interpolation_mode() const = 0;
    virtual WrapMode wrap_modes(size_t i) const = 0;
    virtual FixedArray<float, 4> border_color() const = 0;
    virtual uint32_t layers() const = 0;
};

}
