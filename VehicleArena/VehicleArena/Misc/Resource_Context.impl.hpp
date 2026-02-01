// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include "Resource_Context.hpp"
#include <iostream>
#include <stdexcept>

namespace VA {

template <class TResourceContext>
ResourceContextGuard<TResourceContext>::ResourceContextGuard(TResourceContext& resource_context)
    : resource_context_{resource_context}
    , old_primary_resource_context_{ResourceContextStack<TResourceContext>::primary_resource_context_}
    , old_secondary_resource_context_{ResourceContextStack<TResourceContext>::secondary_resource_context_}
{
    if (old_primary_resource_context_ == nullptr) {
        ResourceContextStack<TResourceContext>::primary_resource_context_ = &resource_context_;
    }
    ResourceContextStack<TResourceContext>::secondary_resource_context_ = &resource_context_;
}

template <class TResourceContext>
ResourceContextGuard<TResourceContext>::~ResourceContextGuard() {
    ResourceContextStack<TResourceContext>::primary_resource_context_ = old_primary_resource_context_;
    ResourceContextStack<TResourceContext>::secondary_resource_context_ = old_secondary_resource_context_;
}

template <class TResourceContext>
TResourceContext& ResourceContextStack<TResourceContext>::primary_resource_context() {
    if (primary_resource_context_ == nullptr) {
        throw std::runtime_error("Primary resource_context on empty stack");
    }
    return *primary_resource_context_;
}

template <class TResourceContext>
TResourceContext& ResourceContextStack<TResourceContext>::resource_context() {
    if (secondary_resource_context_ == nullptr) {
        throw std::runtime_error("Secondary resource_context on empty stack");
    }
    return *secondary_resource_context_;
}

template <class TResourceContext>
std::function<std::function<void()>(std::function<void()>)>
    ResourceContextStack<TResourceContext>::generate_thread_runner(
        TResourceContext& primary_resource_context,
        TResourceContext& secondary_resource_context)
{
    return [primary_resource_context, secondary_resource_context](std::function<void()> f){
        return [primary_resource_context, secondary_resource_context, f](){
            ResourceContextGuard<TResourceContext> rrg0{const_cast<TResourceContext&>(primary_resource_context)};
            ResourceContextGuard<TResourceContext> rrg1{const_cast<TResourceContext&>(secondary_resource_context)};
            f();
        };
    };
}

template <class TResourceContext>
THREAD_LOCAL(TResourceContext*) ResourceContextStack<TResourceContext>::primary_resource_context_ = nullptr;

template <class TResourceContext>
THREAD_LOCAL(TResourceContext*) ResourceContextStack<TResourceContext>::secondary_resource_context_ = nullptr;

}
