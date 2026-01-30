// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Array/Fixed_Array.hpp>
#include <cmath>
#include <stdexcept>
#include <stdexcept>
#include <string>

namespace VA {

#pragma warning( push )
#pragma warning( disable : 4103 )
#include <VehicleArena/Misc/Packed_Begin.hpp>
// 24-bit, 3 * 8 bits
struct Rgb24 {
    unsigned char r;
    unsigned char g;
    unsigned char b;
    Rgb24() {}
    Rgb24(
        unsigned char r,
        unsigned char g,
        unsigned char b)
    :r(r), g(g), b(b) {}
    static inline Rgb24 red() {
        return Rgb24{255, 0, 0};
    }
    static inline Rgb24 green() {
        return Rgb24{0, 255, 0};
    }
    static inline Rgb24 blue() {
        return Rgb24{0, 0, 255};
    }
    static inline Rgb24 black() {
        return Rgb24{0, 0, 0};
    }
    static inline Rgb24 white() {
        return Rgb24{255, 255, 255};
    }
    static inline Rgb24 nan() {
        return Rgb24{255, 0, 255};
    }
    static inline Rgb24 gray() {
        return Rgb24{127, 127, 127};
    }
    static inline Rgb24 yellow() {
        return Rgb24{255, 255, 0};
    }
    static inline Rgb24 orange() {
        return Rgb24{255, 127, 0};
    }
    static inline Rgb24 from_float_grayscale(float grayscale) {
        if (std::isnan(grayscale)) {
            return Rgb24::nan();
        }
        // consider using grayscale.clip(0, 1) if this fails
        if (grayscale < 0) {
            throw std::runtime_error("PpmImage::from_float_grayscale received " + std::to_string(grayscale) + "<0");
        }
        if (grayscale > 1) {
            throw std::runtime_error("PpmImage::from_float_grayscale received " + std::to_string(grayscale) + ">1");
        }
        return Rgb24{
            (unsigned char)(std::round(grayscale * 255)),
            (unsigned char)(std::round(grayscale * 255)),
            (unsigned char)(std::round(grayscale * 255))};
    }
    static inline Rgb24 from_float_rgb(float r, float g, float b) {
        if (std::isnan(r) || std::isnan(g) || std::isnan(b)) {
            if (!std::isnan(r) || !std::isnan(g) || !std::isnan(b)) {
                throw std::runtime_error("PpmImage::from_float_rgb received inconsistent NANs");
            }
            return Rgb24::nan();
        }
        if (r < 0.f || g < 0.f || b < 0.f) {
            throw std::runtime_error("PpmImage::from_float_rgb received value < 0");
        }
        if (r > 1.f || g > 1.f || b > 1.f) {
            throw std::runtime_error("PpmImage::from_float_rgb received value > 1");
        }
        return Rgb24{
            (unsigned char)(std::round(r * 255)),
            (unsigned char)(std::round(g * 255)),
            (unsigned char)(std::round(b * 255))};
    }
    static inline Rgb24 from_float_rgb(const FixedArray<float, 3>& rgb) {
        return from_float_rgb(rgb(0), rgb(1), rgb(2));
    }
} PACKED;
#include <VehicleArena/Misc/Packed_End.hpp>
#pragma warning ( pop )

static_assert(sizeof(Rgb24) == 3);

}
