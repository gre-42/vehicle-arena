// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <cstdint>
#include <sstream>
#include <stdexcept>
#include <string>

namespace VA {

double safe_stod(const std::string& s);
float safe_stof(const std::string& s);
int safe_stoi(const std::string& s);
unsigned int safe_stou(const std::string& s);
uint64_t safe_stou64(const std::string& s);
size_t safe_stoz(const std::string& s);
bool safe_stob(const std::string& s);

template <class TData>
TData safe_sto(const std::string& s);

template <> inline double safe_sto<double>(const std::string& s) { return safe_stod(s); }
template <> inline float safe_sto<float>(const std::string& s) { return safe_stof(s); }
template <> inline int safe_sto<int>(const std::string& s) { return safe_stoi(s); }
template <> inline unsigned int safe_sto<unsigned>(const std::string& s) { return safe_stou(s); }
template <> inline uint64_t safe_sto<uint64_t>(const std::string& s) { return safe_stou64(s); }
// Disabled because it is either "unsigned int" or "uint64_t"
// template <> inline size_t safe_sto<size_t>(const std::string& s) { return safe_stoz(s); }
template <> inline bool safe_sto<bool>(const std::string& s) { return safe_stob(s); }

template <class T>
T safe_stox(const std::string& s, const char* msg = "safe_stox") {
    T res;
    std::stringstream sstr;
    sstr << s;
    sstr >> res;
    if ((sstr.rdbuf()->in_avail() != 0) || sstr.fail()) {
        throw std::runtime_error(msg + std::string(": \"") + s + '"');
    }
    return res;
}

}
