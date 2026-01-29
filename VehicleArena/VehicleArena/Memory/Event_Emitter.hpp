// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <functional>
#include <map>

namespace VA {

template <class... Args>
class EventEmitter;

template <class... Args>
class EventReceiverDeletionToken {
    EventReceiverDeletionToken(const EventReceiverDeletionToken&) = delete;
    EventReceiverDeletionToken& operator = (const EventReceiverDeletionToken&) = delete;
public:
    using Emitter = EventEmitter<Args...>;
    friend Emitter;
    explicit EventReceiverDeletionToken(Emitter& emitter, std::function<void(const Args&...)> f);
    ~EventReceiverDeletionToken();
    void reset();
private:
    Emitter* emitter_;
};

template <class... Args>
class EventEmitter {
public:
    using DeletionToken = EventReceiverDeletionToken<Args...>;
    friend DeletionToken;
    using EventCallback = std::function<void(const Args&...)>;
    using OnInsert = std::function<void(const EventCallback&)>;
    EventEmitter(const EventEmitter&) = delete;
    EventEmitter& operator = (const EventEmitter&) = delete;
    explicit EventEmitter(OnInsert on_insert = OnInsert());
    ~EventEmitter();
    void insert(DeletionToken& deletion_token, EventCallback f);
    void clear();
    void emit(const Args&... args);
private:
    OnInsert on_insert_;
    std::map<DeletionToken*, EventCallback> functions_;
};

}

#include "Event_Emitter.impl.hpp"
