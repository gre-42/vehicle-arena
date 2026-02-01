// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <cstdint>
#include <nlohmann/json_fwd.hpp>
#include <string>

namespace VA {

static const size_t SURFACE_BASE_OFFSET = 22;
static const size_t SURFACE_BASE_NBITS = 5;
static const size_t SURFACE_NBITS = SURFACE_BASE_NBITS + 1;

enum class PhysicsMaterial: uint32_t {
    NONE                        = 0,
    ATTR_VISIBLE                = (1 << 0),
    ATTR_COLLIDE                = (1 << 1),
    ATTR_TWO_SIDED              = (1 << 2),
    ATTR_ALIGN_STRICT           = (1 << 3),
    ATTR_CONVEX                 = (1 << 4),
    ATTR_CONCAVE                = (1 << 5),
    ATTR_CONTAINS_SKIDMARKS     = (1 << 6),
    ATTR_SLIPPERY               = (1 << 7),
    ATTR_LIQUID                 = (1 << 8),
    OBJ_ALIGNMENT_PLANE         = (1 << 9),
    OBJ_CHASSIS                 = (1 << 10),
    OBJ_TIRE_LINE               = (1 << 11),
    OBJ_GRIND_CONTACT           = (1 << 12),
    OBJ_GRIND_LINE              = (1 << 13),
    OBJ_ALIGNMENT_CONTACT       = (1 << 14),
    OBJ_BULLET_LINE_SEGMENT     = (1 << 15),
    OBJ_BULLET_MESH             = (1 << 16),
    OBJ_HITBOX                  = (1 << 17),
    OBJ_DISTANCEBOX             = (1 << 18),
    OBJ_GRASS                   = (1 << 19),
    OBJ_GROUND                  = (1 << 20),
    OBJ_WAY_AIR_SUPPORT         = (1 << 21),
    SURFACE_BASE_TARMAC         = (1 << SURFACE_BASE_OFFSET),       //     1
    SURFACE_BASE_GRAVEL         = (2 << SURFACE_BASE_OFFSET),       //    10
    SURFACE_BASE_SNOW           = (3 << SURFACE_BASE_OFFSET),       //    11
    SURFACE_BASE_ICE            = (4 << SURFACE_BASE_OFFSET),       //   100
    SURFACE_BASE_SAND           = (5 << SURFACE_BASE_OFFSET),       //   101
    SURFACE_BASE_GRASS          = (6 << SURFACE_BASE_OFFSET),       //   110
    SURFACE_BASE_DIRT           = (7 << SURFACE_BASE_OFFSET),       //   111
    SURFACE_BASE_TIRE           = (8 << SURFACE_BASE_OFFSET),       //  1000
    SURFACE_BASE_STONE          = (9 << SURFACE_BASE_OFFSET),       //  1001
    SURFACE_BASE_FOLIAGE        = (10 << SURFACE_BASE_OFFSET),      //  1010
    SURFACE_BASE_METAL          = (11 << SURFACE_BASE_OFFSET),      //  1011
    SURFACE_BASE_CONCRETE       = (12 << SURFACE_BASE_OFFSET),      //  1100
    SURFACE_BASE_GLASS          = (13 << SURFACE_BASE_OFFSET),      //  1101
    SURFACE_BASE_WATER          = (14 << SURFACE_BASE_OFFSET),      //  1110
    SURFACE_BASE_FOOT           = (15 << SURFACE_BASE_OFFSET),      //  1111
    SURFACE_BASE_DESERT_ROAD    = (16 << SURFACE_BASE_OFFSET),      // 10000
    SURFACE_BASE_AVATAR         = (17 << SURFACE_BASE_OFFSET),      // 10001
    SURFACE_WET                 = (1 << (0 + SURFACE_BASE_OFFSET + SURFACE_BASE_NBITS)),

    OBJ_BULLET_MASK = OBJ_BULLET_LINE_SEGMENT | OBJ_BULLET_MESH,
    OBJ_BULLET_COLLIDABLE_MASK = OBJ_CHASSIS | OBJ_HITBOX,

    SURFACE_BASE_MASK = ((1 << SURFACE_BASE_NBITS) - 1) << SURFACE_BASE_OFFSET,
    SURFACE_MASK = ((1 << SURFACE_NBITS) - 1) << SURFACE_BASE_OFFSET
};

inline bool any(PhysicsMaterial a) {
    return a != PhysicsMaterial::NONE;
}

inline PhysicsMaterial operator & (PhysicsMaterial a, PhysicsMaterial b) {
    return (PhysicsMaterial)((uint32_t)a & (uint32_t)b);
}

inline PhysicsMaterial operator | (PhysicsMaterial a, PhysicsMaterial b) {
    return (PhysicsMaterial)((uint32_t)a | (uint32_t)b);
}

inline PhysicsMaterial& operator |= (PhysicsMaterial& a, PhysicsMaterial b) {
    a = a | b;
    return a;
}

inline PhysicsMaterial& operator &= (PhysicsMaterial& a, PhysicsMaterial b) {
    a = a & b;
    return a;
}

inline PhysicsMaterial operator ~ (PhysicsMaterial a) {
    return (PhysicsMaterial)(~(uint32_t)a);
}

PhysicsMaterial physics_material_from_string(const std::string& s);
std::string physics_material_to_string(PhysicsMaterial p);

void from_json(const nlohmann::json& j, PhysicsMaterial& p);

}
