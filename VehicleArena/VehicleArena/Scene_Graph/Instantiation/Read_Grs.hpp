// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Geometry/Primitives/Axis_Aligned_Bounding_Box.hpp>
#include <cstdint>
#include <iosfwd>

namespace VA {

enum class IoVerbosity;

namespace Grs {

struct ResourceGroup {
    std::string name;
    std::vector<std::string> elements;
};

struct Coords16 {
    FixedArray<uint16_t, 3> p = uninitialized;
    uint16_t flags;
};
static_assert(sizeof(Coords16) == 8);

struct Coords8 {
    FixedArray<uint8_t, 3> p = uninitialized;
    uint8_t flags;
};
static_assert(sizeof(Coords8) == 4);

struct Cell {
    AxisAlignedBoundingBox<float, 3> aabb;
    std::vector<Coords16> coords16;
    std::vector<Coords8> coords8;
};

struct Model {
    std::vector<ResourceGroup> resource_groups;
    std::vector<Cell> cells;
};

Model load_grs(std::istream& istr, IoVerbosity verbosity);
Model load_grs(const std::string& filename, IoVerbosity verbosity);

}
}
