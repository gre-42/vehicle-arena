// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "StbImage1.hpp"
#include <VehicleArena/Images/Draw_Generic.hpp>
#include <VehicleArena/Stats/Min_Max.hpp>
#include <fstream>
#include <stb/stb_image_write.h>
#include <stb_cpp/stb_image_load.hpp>
#include <stdexcept>

using namespace VA;

StbImage1::StbImage1() = default;

StbImage1::~StbImage1() = default;

StbImage1::StbImage1(const ArrayShape& shape, uint8_t color)
    : Array<uint8_t>(shape)
{
    Array<uint8_t>& t = *this;
    t = color;
}

StbImage1::StbImage1(const Array<uint8_t>& other)
    : Array<uint8_t>(other)
{}

StbImage1::StbImage1(const ArrayShape& shape)
    : Array<uint8_t>(shape) {}

StbImage1::StbImage1(const StbInfo<uint8_t>& stb_info) {
    if (stb_info.nrChannels != 1) {
        throw std::runtime_error("Image does not have 1 channel");
    }
    resize((size_t)stb_info.height, (size_t)stb_info.width);
    memcpy(flat_begin(), stb_info.data(), nbytes());
}

StbImage1 StbImage1::T() const {
    const Array<uint8_t>& t = *this;
    return StbImage1(t.T());
}

StbImage1 StbImage1::reversed(size_t axis) const {
    const Array<uint8_t>& t = *this;
    return StbImage1(t.reversed());
}

void StbImage1::draw_fill_rect(const FixedArray<size_t, 2>& center, size_t size, uint8_t color) {
    VA::draw_fill_rect(*this, center, size, color);
}

void StbImage1::draw_empty_rect(const FixedArray<size_t, 2>& center, size_t size, uint8_t color) {
    VA::draw_empty_rect(*this, center, size, color);
}

void StbImage1::draw_line(
    const FixedArray<float, 2>& from,
    const FixedArray<float, 2>& to,
    size_t thickness,
    uint8_t color,
    const uint8_t* short_line_color)
{
    draw_line_ext(from, to, thickness, color, false, short_line_color); // false = infinite
}

void StbImage1::draw_infinite_line(
    const FixedArray<float, 2>& from,
    const FixedArray<float, 2>& to,
    size_t thickness,
    uint8_t color,
    const uint8_t* short_line_color)
{
    draw_line_ext(from, to, thickness, color, true, short_line_color); // true = infinite
}

void StbImage1::draw_line_ext(
    const FixedArray<float, 2>& from,
    const FixedArray<float, 2>& to,
    size_t thickness,
    uint8_t color,
    bool infinite,
    const uint8_t* short_line_color)
{
    VA::draw_line_ext(
        *this,
        from,
        to,
        thickness,
        color,
        infinite,
        short_line_color);
}

void StbImage1::draw_mask(const Array<bool>& mask, uint8_t color) {
    assert(all(mask.shape() == shape()));
    Array<uint8_t> f = flattened();
    Array<bool> m = mask.flattened();
    for (size_t i = 0; i < f.length(); ++i) {
        if (!m(i)) {
            f(i) = color;
        }
    }
}

void StbImage1::draw_streamline(
    const FixedArray<size_t, 2>& center,
    const Array<float>& velocity,
    size_t size,
    size_t length,
    uint8_t color)
{
    visit_streamline(FixedArray<size_t, 2>{ shape(0), shape(1) }, center, velocity, length, [&](const FixedArray<size_t, 2>& ipos){
        draw_fill_rect(ipos, size, color);
    });
}

StbImage1 StbImage1::load_from_file(const std::string& filename) {
    auto image = stb_load8(filename, FlipMode::NONE);
    if (image.nrChannels != 1) {
        throw std::runtime_error("Image does not have 1 channel: \"" + filename + '"');
    }
    return StbImage1{image};
}

void StbImage1::save_to_file(const std::string& filename, int jpg_quality) const {
    if (filename.ends_with(".png")) {
        if (!stbi_write_png(filename.c_str(), (int)shape(1), (int)shape(0), 1, flat_begin(), 0)) {
            throw std::runtime_error("Could not save to file " + filename);
        }
    } else if (filename.ends_with(".jpg")) {
        if (!stbi_write_jpg(filename.c_str(), (int)shape(1), (int)shape(0), 1, flat_begin(), jpg_quality)) {
            throw std::runtime_error("Could not save to file " + filename);
        }
    } else {
        throw std::runtime_error("Filename does not have png or jpg extension: \"" + filename + '"');
    }
}

StbImage1 StbImage1::from_float_grayscale(const Array<float>& grayscale) {
    if (grayscale.ndim() != 2) {
        throw std::runtime_error("from_float_grayscale: grayscale image does not have ndim=2, but " + grayscale.shape().str());
    }
    StbImage1 result(grayscale.shape());
    Array<uint8_t> f = result.flattened();
    Array<float> g = grayscale.flattened();
    for (size_t i = 0; i < g.length(); i++) {
        f(i) = uint8_from_float(g(i));
    }
    return result;
}

Array<float> StbImage1::to_float_grayscale() const {
    Array<float> grayscale(shape());
    Array<uint8_t> f = flattened();
    Array<float> g = grayscale.flattened();
    for (size_t i = 0; i < g.length(); i++) {
        g(i) = static_cast<float>(f(i)) / 255.f;
        assert(g(i) >= 0);
        assert(g(i) <= 1);
    }
    return grayscale;
}
