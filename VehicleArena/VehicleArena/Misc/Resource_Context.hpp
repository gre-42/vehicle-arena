// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Threads/Thread_Local.hpp>
#include <functional>
#include <iosfwd>
#include <list>

namespace VA {

template <class TResourceContext>
class ResourceContextGuard {
    ResourceContextGuard(const ResourceContextGuard &) = delete;
    ResourceContextGuard &operator=(const ResourceContextGuard &) = delete;

public:
    explicit ResourceContextGuard(TResourceContext &resource_context);
    ~ResourceContextGuard();

private:
    TResourceContext resource_context_;
    TResourceContext *old_primary_resource_context_;
    TResourceContext *old_secondary_resource_context_;
};

template <class TResourceContext>
class ResourceContextStack {
    friend ResourceContextGuard<TResourceContext>;
    ResourceContextStack() = delete;

public:
    static TResourceContext& primary_resource_context();
    static TResourceContext& resource_context();
    static std::function<std::function<void()>(std::function<void()>)>
        generate_thread_runner(
            TResourceContext& primary_resource_context,
            TResourceContext& secondary_resource_context);
protected:
    static THREAD_LOCAL(TResourceContext*) primary_resource_context_;
    static THREAD_LOCAL(TResourceContext*) secondary_resource_context_;
};

}
