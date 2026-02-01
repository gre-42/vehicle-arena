// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Load_Heightmap_From_File.hpp"
#include <VehicleArena/Geography/Heightmaps/Cities_Skylines.hpp>
#include <VehicleArena/Geography/Heightmaps/Terrarium.hpp>
#include <VehicleArena/Images/Pgm_Image.hpp>
#include <stb_cpp/stb_array.hpp>
#include <stb_cpp/stb_image_load.hpp>

using namespace VA;

template <class TData>
Array<TData> VA::load_heightmap_from_file(const std::string& filename) {
    if (filename.ends_with(".pgm")) {
        return cities_skylines_to_meters<TData>(PgmImage::load_from_file(filename));
    }
    auto imX = stb_load(filename, FlipMode::NONE);
    if (auto* im8 = std::get_if<StbInfo<uint8_t>>(&imX); im8 != nullptr) {
        auto ar = stb_image_2_array(*im8);
        if (ar.shape(0) == 3) {
            return terrarium_to_meters<TData>(ar);
        } else if (ar.shape(0) == 1) {
            return ar[0].template casted<TData>() / TData(255);
        } else {
            throw std::runtime_error("Height map is no 16-bit image and does not have 1 or 3 channels");
        }
    }
    if (auto* im16 = std::get_if<StbInfo<uint16_t>>(&imX); im16 != nullptr) {
        auto ar = stb_image_2_array(*im16);
        if (ar.shape(0) == 1) {
            return cities_skylines_to_meters<TData>(ar);
        } else {
            throw std::runtime_error("Height map is a 16-bit image and does not have exactly 1 channel");
        }
    }
    throw std::runtime_error("Image does not have 8 or 16 bit");
}

template Array<float> VA::load_heightmap_from_file<float>(const std::string&);
template Array<double> VA::load_heightmap_from_file<double>(const std::string&);
