// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Track_Writer.hpp"
#include <VehicleArena/Os/Os.hpp>
#include <VehicleArena/Physics/Misc/Track_Element.hpp>
#include <stdexcept>

using namespace VA;

TrackWriter::TrackWriter(
    const std::string& filename,
    const TransformationMatrix<double, double, 3>* geographic_mapping)
    : filename_{ filename }
    , geographic_mapping_{ geographic_mapping }
    , ofstr_{ create_ofstream(filename) }
{
    if (ofstr_->fail()) {
        throw std::runtime_error("Could not open track file for write \"" + filename + '"');
    }
}

TrackWriter::~TrackWriter() = default;

void TrackWriter::write(const TrackElement& e)
{
    if (geographic_mapping_ == nullptr) {
        throw std::runtime_error("TrackWriter::write without geographic mapping");
    }
    e.write_to_stream(*ofstr_, *geographic_mapping_);
    *ofstr_ << '\n';
}

void TrackWriter::flush() {
    ofstr_->flush();
    if (ofstr_->fail()) {
        throw std::runtime_error("Could not write to file " + filename_);
    }
}
