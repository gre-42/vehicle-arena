// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <Mlib/Io/Binary.hpp>

namespace VA {

class BinaryReader {
public:
    inline BinaryReader(std::istream& istr, IoVerbosity verbosity)
        : istr_{ istr }
        , verbosity_{ verbosity }
    {}
    inline std::string read_string(const char* message) {
        auto len = VA::read_binary<uint32_t>(istr_, message, verbosity_);
        return VA::read_string(istr_, len, message, verbosity_);
    }
    template <class T>
    inline T read_binary(const char* message) {
        return VA::read_binary<T>(istr_, message, verbosity_);
    }
    void seek_relative_positive(std::streamoff amount) {
        VA::seek_relative_positive(istr_, amount, verbosity_);
    }

private:
    std::istream& istr_;
    IoVerbosity verbosity_;
};

}
