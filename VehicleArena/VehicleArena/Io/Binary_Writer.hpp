// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <Mlib/Io/Binary.hpp>
#include <cstdint>

namespace VA {

class BinaryWriter {
public:
    inline explicit BinaryWriter(std::ostream& ostr)
        : ostr_{ ostr }
    {}
    inline void write_string(const std::string& str, const char* message) {
        VA::write_binary(ostr_, integral_cast<uint32_t>(str.length()), message);
        VA::write_iterable(ostr_, str, message);
    }
    template <class T>
    inline void write_binary(const T& v, const char* message) {
        return VA::write_binary(ostr_, v, message);
    }
private:
    std::ostream& ostr_;
};

}
