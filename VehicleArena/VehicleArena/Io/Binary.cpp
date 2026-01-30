// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Binary.hpp"
#include <VehicleArena/Os/Os.hpp>

using namespace VA;

void VA::print_char(char c) {
    char v = (c >= ' ') && (c <= '~') ? c : '.';
    linfo() << "Read: " << std::hex << "0x" << std::setfill('0') << std::setw(2) << (uint32_t)(uint8_t)c << " - " << v;
}

void VA::print_chars(std::span<char> span, const char* msg) {
    if (msg != nullptr) {
        linfo() << "Msg: " << msg;
    }
    for (char c : span) {
        print_char(c);
    }
}

std::string VA::read_string(std::istream& istr, size_t length, const char* msg, IoVerbosity verbosity) {
    if (length > 1'000) {
        throw std::runtime_error("String too large: " + std::string(msg));
    }
    std::string s(length, '?');
    read_vector(istr, s, msg, verbosity);
    return s;
}

void VA::seek_relative_positive(std::istream& istr, std::streamoff amount, IoVerbosity verbosity) {
    if (amount < 0) {
        throw std::runtime_error("Seek in negative direction");
    }
    if (any(verbosity & IoVerbosity::DATA)) {
        for (std::streamoff i = 0; i < amount; ++i) {
            auto c = istr.get();
            if (c == EOF) {
                throw std::runtime_error("Could not read char");
            }
            print_char((char)c);
        }
    } else {
        istr.seekg(amount, std::ios::cur);
        if (istr.fail()) {
            throw std::runtime_error("Seekg failed");
        }
    }
}
