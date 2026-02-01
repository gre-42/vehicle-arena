// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Remote/Remote_Site_Id.hpp>
#include <chrono>
#include <cstdint>
#include <iosfwd>
#include <optional>

namespace VA {

struct LocalSceneLevel;
struct RemoteObjectId;
enum class IoVerbosity;
enum class TransmittedFields: uint32_t;

enum class TransmissionHistory: uint32_t {
    NONE = 0,
    SITE_ID = 1 << 1,
    END = 1 << 2
};

inline bool any(TransmissionHistory fields) {
    return fields != TransmissionHistory::NONE;
}

inline TransmissionHistory operator & (TransmissionHistory a, TransmissionHistory b) {
    return (TransmissionHistory)((int)a & (int)b);
}

inline TransmissionHistory operator | (TransmissionHistory a, TransmissionHistory b) {
    return (TransmissionHistory)((int)a | (int)b);
}

inline TransmissionHistory& operator |= (TransmissionHistory& a, TransmissionHistory b) {
    (int&)a |= (int)b;
    return a;
}

class TransmissionHistoryReader {
public:
    explicit TransmissionHistoryReader(
        const LocalSceneLevel& home_scene_level,
        std::chrono::steady_clock::time_point base_time);
    ~TransmissionHistoryReader();
    RemoteObjectId read_remote_object_id(
        std::istream& istr,
        TransmittedFields transmitted_fields,
        IoVerbosity verbosity);
    std::chrono::steady_clock::time_point read_time(
        std::istream& istr,
        IoVerbosity verbosity) const;
    const LocalSceneLevel& home_scene_level;
private:
    std::chrono::steady_clock::time_point base_time_;
    std::optional<RemoteSiteId> site_id_;
};

class TransmissionHistoryWriter {
public:
    explicit TransmissionHistoryWriter(
        std::chrono::steady_clock::time_point base_time);
    ~TransmissionHistoryWriter();
    void write_remote_object_id(
        std::ostream& ostr,
        const RemoteObjectId& remote_object_id,
        TransmittedFields transmitted_fields);
    void write_time(
        std::ostream& ostr,
        std::chrono::steady_clock::time_point time) const;
private:
    std::chrono::steady_clock::time_point base_time_;
    TransmissionHistory history_;
};

}
