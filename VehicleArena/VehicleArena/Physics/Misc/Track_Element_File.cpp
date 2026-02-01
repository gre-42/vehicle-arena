// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Track_Element_File.hpp"
#include <VehicleArena/Physics/Misc/Track_Element_Extended.hpp>
#include <istream>

using namespace VA;

TrackElementFile::TrackElementFile(
    std::unique_ptr<std::istream>&& istr,
    std::string filename)
: istr_{std::move(istr)},
  filename_{std::move(filename)}
{
    if (istr_->fail()) {
        if (filename_.empty()) {
            throw std::runtime_error("Could not open track stream");
        } else {
            throw std::runtime_error("Could not open track reader file \"" + filename_ + '"');
        }
    }
}

TrackElementFile::~TrackElementFile() = default;

TrackElementExtended TrackElementFile::read(
    const std::optional<TrackElementExtended>& predecessor,
    const TransformationMatrix<double, double, 3>& inverse_geographic_mapping,
    size_t ntransformations)
{
    auto result = TrackElementExtended::create(
        predecessor,
        TrackElement::from_stream(*istr_, inverse_geographic_mapping, ntransformations));
    if (istr_->fail() && !istr_->eof()) {
        if (filename_.empty()) {
            throw std::runtime_error("Could not read from track stream");
        } else {
            throw std::runtime_error("Could not read from file \"" + filename_ + '"');
        }
    }
    return result;
}

bool TrackElementFile::eof() const {
    return istr_->eof();
}

void TrackElementFile::restart() {
    istr_->clear();
    istr_->seekg(0);
}
