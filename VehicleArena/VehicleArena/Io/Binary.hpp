// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <Mlib/Memory/Integral_Cast.hpp>
#include <Mlib/Throw_Or_Abort.hpp>
#include <istream>
#include <ostream>
#include <span>
#include <string>
#include <vector>

namespace VA {

enum class IoVerbosity {
    SILENT = 0,
    DATA = 1 << 0,
    METADATA = 1 << 1
};

inline IoVerbosity operator & (IoVerbosity a, IoVerbosity b) {
    return (IoVerbosity)((int)a & (int)b);
}

inline IoVerbosity operator | (IoVerbosity a, IoVerbosity b) {
    return (IoVerbosity)((int)a | (int)b);
}

inline IoVerbosity& operator |= (IoVerbosity& a, IoVerbosity b) {
    (int&)a |= (int)b;
    return a;
}

inline bool any(IoVerbosity v) {
    return v != IoVerbosity::SILENT;
}

void print_char(char c);
void print_chars(std::span<char> span, const char* msg = nullptr);

template <class T>
T read_binary(std::istream& istr, const char* msg, IoVerbosity verbosity) {
    T result;
    istr.read(reinterpret_cast<char*>(&result), sizeof(result));
    if (istr.fail()) {
        THROW_OR_ABORT("Could not read " + std::string(msg) + " from stream");
    }
    if (any(verbosity & IoVerbosity::DATA)) {
        char* begin = reinterpret_cast<char*>(&result);
        char* end = begin + sizeof(result);
        print_chars({ begin, end }, msg);
    }
    return result;
}

template <class TData>
void read_vector(std::istream& istr, const std::span<TData>& vec, const char* msg, IoVerbosity verbosity) {
    read_vector(istr, const_cast<std::span<TData>&>(vec), msg, verbosity);
}

template <class TVec>
void read_vector(std::istream& istr, TVec& vec, const char* msg, IoVerbosity verbosity) {
    istr.read(reinterpret_cast<char*>(vec.data()), integral_cast<std::streamsize>(sizeof(typename TVec::value_type) * vec.size()));
    if (istr.fail()) {
        THROW_OR_ABORT("Could not read vector from stream: " + std::string(msg));
    }
    if (any(verbosity & IoVerbosity::DATA)) {
        char* begin = reinterpret_cast<char*>(vec.data());
        char* end = begin + sizeof(vec[0]) * vec.size();
        print_chars({ begin, end }, msg);
    }
}

std::string read_string(std::istream& istr, size_t length, const char* msg, IoVerbosity verbosity);

void seek_relative_positive(std::istream& str, std::streamoff amount, IoVerbosity verbosity);

template <class T>
void write_binary(std::ostream& ostr, const T& value, const char* msg) {
    ostr.write((const char*)&value, sizeof(value));
    if (ostr.fail()) {
        THROW_OR_ABORT("Could not write " + std::string(msg) + "to stream");
    }
}

template <class TIterable>
void write_iterable(std::ostream& ostr, const TIterable& iterable, const char* msg) {
    for (const auto& e : iterable) {
        write_binary(ostr, e, msg);
    }
}

}
