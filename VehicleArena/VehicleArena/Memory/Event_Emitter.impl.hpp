// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include "Event_Emitter.hpp"
#include <VehicleArena/Os/Os.hpp>
#include <stdexcept>

namespace VA {

template <class... Args>
EventReceiverDeletionToken<Args...>::EventReceiverDeletionToken(
    Emitter& emitter,
    std::function<void(const Args&...)> f)
    : emitter_{ &emitter }
{
    emitter.insert(*this, std::move(f));
}

template <class... Args>
EventReceiverDeletionToken<Args...>::~EventReceiverDeletionToken() {
    reset();
}

template <class... Args>
void EventReceiverDeletionToken<Args...>::reset() {
    if (emitter_ == nullptr) {
        return;
    }
    if (emitter_->functions_.erase(this) != 1) {
        verbose_abort("Could not delete event function");
    }
    emitter_ = nullptr;
}

template <class... Args>
EventEmitter<Args...>::EventEmitter(OnInsert on_insert)
    : on_insert_{ std::move(on_insert) }
{}

template <class... Args>
EventEmitter<Args...>::~EventEmitter() {
    clear();
}

template <class... Args>
void EventEmitter<Args...>::clear() {
    for (const auto& [e, _]: functions_) {
        e->emitter_ = nullptr;
    }
}

template <class... Args>
void EventEmitter<Args...>::insert(
    DeletionToken& deletion_token,
    EventCallback f)
{
    if (on_insert_) {
        on_insert_(f);
    }
    if (!functions_.try_emplace(&deletion_token, std::move(f)).second) {
        throw std::runtime_error("Deletion token already exists");
    }
}

template <class... Args>
void EventEmitter<Args...>::emit(const Args&... args) {
    for (const auto& [e, f] : functions_) {
        if (e->emitter_ != nullptr) {
            f(args...);
        }
    }
}

}
