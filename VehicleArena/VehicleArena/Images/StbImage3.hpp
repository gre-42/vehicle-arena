// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Array/Array.hpp>
#include <VehicleArena/Images/Rgb24.hpp>
#include <cstdint>
#include <string>

template <class TData>
class StbInfo;

namespace VA {

class StbImage3: public Array<Rgb24> {
public:
    StbImage3();
    ~StbImage3();
    explicit StbImage3(const FixedArray<size_t, 2>& shape, const Rgb24& color);
    explicit StbImage3(const Array<Rgb24>& other);
    explicit StbImage3(const FixedArray<size_t, 2>& shape);
    explicit StbImage3(const StbInfo<uint8_t>& stb_info);

    StbImage3 T() const;
    StbImage3 reversed(size_t axis) const;

    void draw_fill_rect(const FixedArray<size_t, 2>& center, size_t size, const Rgb24& color);
    void draw_empty_rect(const FixedArray<size_t, 2>& center, size_t size, const Rgb24& color);
    void draw_line(const FixedArray<float, 2>& from, const FixedArray<float, 2>& to, size_t thickness, const Rgb24& color, const Rgb24* short_line_color = nullptr);
    void draw_infinite_line(const FixedArray<float, 2>& from, const FixedArray<float, 2>& to, size_t thickness, const Rgb24& color, const Rgb24* short_line_color = nullptr);
    void draw_mask(const Array<bool>& mask, const Rgb24& color);
    void draw_streamline(const FixedArray<size_t, 2>& center, const Array<float>& velocity, size_t size, size_t length, const Rgb24& color);

    static StbImage3 load_from_file(const std::string& filename);

    void save_to_file(const std::string &filename, int jpg_quality = 95) const;

    static StbImage3 from_rgb(const Array<uint8_t>& grayscale);
    static StbImage3 from_float_rgb(const Array<float>& grayscale);
    static StbImage3 from_float_grayscale(const Array<float>& grayscale);

    Array<float> to_float_grayscale() const;
    Array<float> to_float_rgb() const;
private:
    void draw_line_ext(const FixedArray<float, 2>& from, const FixedArray<float, 2>& to, size_t thickness, const Rgb24& color, bool infinite, const Rgb24* short_line_color = nullptr);
};

}
