// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Track_Writer_Gpx.hpp"
#include <VehicleArena/Physics/Misc/Track_Element.hpp>
#include <stdexcept>

using namespace VA;

TrackWriterGpx::TrackWriterGpx(const std::string& filename)
: filename_{filename},
  ofstr_{create_ofstream(filename)}
{
    if (ofstr_->fail()) {
        throw std::runtime_error("Could not open gpx file for write \"" + filename + '"');
    }
    ofstr_->precision(10);
    *ofstr_ << R"(<?xml version="1.0" encoding="UTF-8" standalone="no" ?>
<gpx version="1.1" creator="Vanilla Rally">
  <trk>
    <trkseg>
)";
    if (ofstr_->fail()) {
        throw std::runtime_error("Could not write to file " + filename);
    }
}

TrackWriterGpx::~TrackWriterGpx() {
    *ofstr_ << R"(    </trkseg>
  </trk>
</gpx>)";
    ofstr_->flush();
    if (ofstr_->fail()) {
        lerr() << "Could not write to file "  << filename_;
    }
}

void TrackWriterGpx::write(const FixedArray<double, 3>& position) {
    *ofstr_ << "      <trkpt lat=\"" << position(0) << "\" lon=\"" << position(1) << "\">\n";
    *ofstr_ << "        <ele>" << position(2) << "</ele>\n";
    *ofstr_ << "      </trkpt>\n";
    if (ofstr_->fail()) {
        throw std::runtime_error("Could not write to file " + filename_);
    }
}

void TrackWriterGpx::flush() {
    ofstr_->flush();
    if (ofstr_->fail()) {
        throw std::runtime_error("Could not write to file " + filename_);
    }
}
