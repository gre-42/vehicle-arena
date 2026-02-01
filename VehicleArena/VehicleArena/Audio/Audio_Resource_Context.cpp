// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Audio_Resource_Context.hpp"
#include <VehicleArena/Misc/Resource_Context.impl.hpp>
#include <VehicleArena/Audio/Audio_Resources.hpp>

using namespace VA;

// template<>
// thread_local std::list<AudioResourceContext> ResourceContextStack<AudioResourceContext>::resource_context_stack_;

AudioResourceContext::AudioResourceContext()
: audio_resources{ std::make_shared<AudioResources>() }
{}

AudioResourceContext::~AudioResourceContext() = default;

std::shared_ptr<AudioResources> AudioResourceContextStack::primary_audio_resources() {
    return primary_resource_context().audio_resources;
}

std::shared_ptr<AudioResources> AudioResourceContextStack::audio_resources() {
    return resource_context().audio_resources;
}

template ResourceContextGuard<const AudioResourceContext>::ResourceContextGuard(const AudioResourceContext& resource_context);
template ResourceContextGuard<const AudioResourceContext>::~ResourceContextGuard();

template const AudioResourceContext* ResourceContextStack<const AudioResourceContext>::primary_resource_context_;
template const AudioResourceContext* ResourceContextStack<const AudioResourceContext>::secondary_resource_context_;
template const AudioResourceContext& ResourceContextStack<const AudioResourceContext>::primary_resource_context();
template const AudioResourceContext& ResourceContextStack<const AudioResourceContext>::resource_context();
template std::function<std::function<void()>(std::function<void()>)>
    ResourceContextStack<const AudioResourceContext>::generate_thread_runner(
        const AudioResourceContext &primary_resource_context,
        const AudioResourceContext &secondary_resource_context);
