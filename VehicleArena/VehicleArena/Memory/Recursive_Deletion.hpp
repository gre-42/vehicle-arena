// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Threads/Unlock_Guard.hpp>

namespace VA {

template <class TContainer>
void clear_container_recursively(TContainer& container) {
    while (!container.empty()) {
        container.erase(container.begin());
    }
}

template <class TContainer, class TFunction>
void clear_map_recursively(TContainer& container, const TFunction& deleter) {
    while (!container.empty()) {
        auto node = container.extract(container.begin());
        deleter(node);
    }
}

template <class TContainer, class TFunction>
void clear_set_recursively(TContainer& container, const TFunction& deleter) {
    while (!container.empty()) {
        auto node = container.extract(container.begin());
        deleter(node.value());
    }
}

template <class TContainer, class TLock, class TFunction>
void clear_set_recursively_with_lock(
    TContainer& container,
    TLock& lock,
    const TFunction& deleter)
{
    while (!container.empty()) {
        auto node = container.extract(container.begin());
        UnlockGuard ulock{ lock };
        deleter(node.value());
    }
}

template <class TContainer, class TFunction>
void clear_list_recursively(TContainer& elements, const TFunction& deleter) {
    while (!elements.empty()) {
        auto it_second = elements.begin();
        ++it_second;
        TContainer list2;
        list2.splice(list2.begin(), elements, elements.begin(), it_second);
        deleter(list2.front());
    }
}

template <class TContainer, class TLock, class TFunction>
void clear_list_recursively_with_lock(
    TContainer& elements,
    TLock& lock,
    const TFunction& deleter)
{
    while (!elements.empty()) {
        auto it_second = elements.begin();
        ++it_second;
        TContainer list2;
        list2.splice(list2.begin(), elements, elements.begin(), it_second);
        UnlockGuard ulock{ lock };
        deleter(list2.front());
        list2.clear();
    }
}

}
