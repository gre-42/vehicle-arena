// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Float_128_Io.hpp"
#include <iostream>
#include <quadmath.h>
#include <stdexcept>

using namespace VA;

std::istream& std::operator >> (std::istream& istr, float128& f) {
    std::string s;
    istr >> s;
    const char* cs = s.c_str();
    char *sp;
    f = strtoflt128(cs, &sp);
    if ((size_t)(sp - cs) != s.length()) {
        throw std::runtime_error("Could not parse string as float128: \"" + s + '"');
    }
    return istr;
}

std::ostream& std::operator << (std::ostream& ostr, const float128& f) {
    // From: https://www.boost.org/doc/libs/1_65_0/boost/multiprecision/float128.hpp
    //       int max_digits10 = 36;
    char buf[128];
    int n = quadmath_snprintf (buf, sizeof buf, "%.36Qe", f);
    if (((size_t) n >= sizeof buf) || (n < 0)) {
        throw std::runtime_error("Could not convert float128 to string");
    }
    ostr << buf;
    return ostr;
}
