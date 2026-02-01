// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Hashing/Hash.hpp>
#include <VehicleArena/Hashing/Std_Hash.hpp>
#include <VehicleArena/Remote/Remote_Site_Id.hpp>
#include <compare>
#include <iosfwd>
#include <string>

namespace VA {

using LocalObjectId = uint64_t;

struct RemoteObjectId {
    RemoteSiteId site_id;
    LocalObjectId object_id;
    std::string to_string() const;
    std::string to_displayname() const;
    std::strong_ordering operator <=> (const RemoteObjectId&) const = default;
};

std::ostream& operator << (std::ostream& ostr, RemoteObjectId id);

}

template <>
struct std::hash<VA::RemoteObjectId>
{
    std::size_t operator() (const VA::RemoteObjectId& a) const {
        return VA::hash_combine(
            a.site_id,
            a.object_id);
    }
};
