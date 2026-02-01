// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Memory/Dangling_Base_Class.hpp>
#include <VehicleArena/Memory/Destruction_Notifier.hpp>
#include <cstdint>
#include <iosfwd>

namespace VA {

struct RemoteObjectId;
class TransmissionHistoryReader;
class TransmissionHistoryWriter;
enum class KnownFields: uint32_t;
enum class TransmittedFields: uint32_t;
enum class ProxyTasks;

class IIncrementalObject: public virtual DestructionNotifier, public virtual DanglingBaseClass {
public:
    virtual ~IIncrementalObject() = default;
    virtual void read(
        std::istream& istr,
        const RemoteObjectId& remote_object_id,
        ProxyTasks proxy_tasks,
        TransmittedFields transmitted_fields,
        TransmissionHistoryReader& transmission_history_reader) = 0;
    virtual void write(
        std::ostream& ostr,
        const RemoteObjectId& remote_object_id,
        ProxyTasks proxy_tasks,
        KnownFields known_fields,
        TransmissionHistoryWriter& transmission_history_writer) = 0;
};

}
