// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "String_View_To_Number.hpp"
#include <cmath>
#include <stdexcept>

using namespace VA;

double VA::safe_stod(const std::string_view& s) {
    if (s == "inf") {
        return INFINITY;
    }
    if (s == "-inf") {
        return -INFINITY;
    }
    if (s == "nan") {
        return NAN;
    }
    return safe_stox<double>(s, "double");
}

float VA::safe_stof(const std::string_view& s) {
    if (s == "inf") {
        return INFINITY;
    }
    if (s == "-inf") {
        return -INFINITY;
    }
    if (s == "nan") {
        return NAN;
    }
    return safe_stox<float>(s, "float");
}

int VA::safe_stoi(const std::string_view& s) {
    return safe_stox<int>(s, "int");
}

unsigned int VA::safe_stou(const std::string_view& s) {
    return safe_stox<unsigned int>(s, "uint");
}

uint64_t VA::safe_stou64(const std::string_view& s) {
    return safe_stox<uint64_t>(s, "uint64");
}

size_t VA::safe_stoz(const std::string_view& s) {
    return safe_stox<size_t>(s, "size_t");
}

bool VA::safe_stob(const std::string_view& s) {
    if (s == "0") {
        return false;
    }
    if (s == "1") {
        return true;
    }
    throw std::runtime_error("Could not convert \"" + std::string{s} + "\" to bool");
}
