// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Remote/Remote_Site_Id.hpp>
#include <compare>
#include <cstddef>
#include <optional>

namespace VA {

class RemoteObserver;

class ViewableRemoteObject {
    friend RemoteObserver;
public:
    static inline ViewableRemoteObject all() {
        return {};
    }
    ViewableRemoteObject() = default;
    ViewableRemoteObject(
        const std::optional<RemoteSiteId>& site_id,
        const std::optional<uint32_t>& user_id)
        : site_id_{ site_id }
        , user_id_{ user_id }
    {}
private:
    std::optional<RemoteSiteId> site_id_;
    std::optional<uint32_t> user_id_;
};

class RemoteObserver {
public:
    static inline RemoteObserver all() {
        return {};
    }
    RemoteObserver() = default;
    RemoteObserver(
        const std::optional<RemoteSiteId>& site_id,
        const std::optional<uint32_t>& user_id)
        : site_id_{ site_id }
        , user_id_{ user_id }
    {}
    inline bool can_see(const ViewableRemoteObject& obj) const;
    std::strong_ordering operator <=> (const RemoteObserver&) const = default;
private:
    std::optional<RemoteSiteId> site_id_;
    std::optional<uint32_t> user_id_;
};

inline bool RemoteObserver::can_see(const ViewableRemoteObject& obj) const
{
    if (site_id_.has_value() &&
        obj.site_id_.has_value() &&
        (*site_id_ != *obj.site_id_))
    {
        return false;
    }
    if (user_id_.has_value() &&
        obj.user_id_.has_value() &&
        (*user_id_ != *obj.user_id_))
    {
        return false;
    }
    return true;
}

}
