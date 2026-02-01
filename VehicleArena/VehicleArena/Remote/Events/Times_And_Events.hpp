// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Scene_Config/Remote_Event_History_Duration.hpp>
#include <map>

namespace VA {

template <class TTime, class TEvent>
class TimesAndEvents {
public:
    template <class K, class... Args>
    decltype(auto) try_emplace(K&& k, Args&&... args) {
        return events_.try_emplace(std::forward<K>(k), std::forward<Args...>(args...));
    }
    void forget_old_entries(TTime local_time) {
        std::erase_if(events_, [&](const auto& item){
            return item.first + REMOTE_EVENT_HISTORY_DURATION < local_time;
        });
    }
    size_t size() const {
        return events_.size();
    }
    decltype(auto) begin() const {
        return events_.begin();
    }
    decltype(auto) end() const {
        return events_.end();
    }
private:
    std::map<TTime, TEvent> events_;
};

}
