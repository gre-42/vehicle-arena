// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Dds_Info.hpp"
#include <VehicleArena/Images/Dds_Header.hpp>
#include <VehicleArena/Memory/Integral_Cast.hpp>
#include <VehicleArena/Os/Os.hpp>
#include <cstdint>
#include <sstream>
#include <stdexcept>

using namespace VA;

DdsInfo DdsInfo::load_from_file(const std::string& filename) {
    auto ifstr = create_ifstream(filename, std::ios_base::binary);
    if (ifstr->fail()) {
        throw std::runtime_error("Could not open file \"" + filename + "'");
    }
    try {
        return load_from_stream(*ifstr);
    } catch (const std::exception& e) {
        throw std::runtime_error("Could not load \"" + filename + "\": " + e.what());
    }
}

DdsInfo DdsInfo::load_from_stream(std::istream& istream) {
    uint32_t actual_dds_magic;
    istream.read((char*)&actual_dds_magic, 4);
    if (istream.fail()) {
        throw std::runtime_error("Could not read DDS magic key");
    }
    if (actual_dds_magic != DDS_MAGIC) {
        throw std::runtime_error("Invalid DDS magic key");
    }
    DdsHeader header;
    istream.read((char*)&header, sizeof(header));
    if (istream.fail()) {
        throw std::runtime_error("Could not read DDS header");
    }
    // DdsImage result;
    // result.resize(header.width, header.height);
    return DdsInfo{
        .width = integral_cast<int>(header.width),
        .height = integral_cast<int>(header.height)};
}

DdsInfo DdsInfo::load_from_buffer(const std::vector<std::byte>& buffer) {
    if (buffer.size() < sizeof(DdsHeader) + sizeof(uint32_t)) {
        throw std::runtime_error("DDS buffer too small");
    }
    std::stringstream sstr;
    for (size_t i = 0; i < sizeof(DdsHeader) + sizeof(uint32_t); ++i) {
        sstr << (char)buffer[i];
    }
    return load_from_stream(sstr);
}
