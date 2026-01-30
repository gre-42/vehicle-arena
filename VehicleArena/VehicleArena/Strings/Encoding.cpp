// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Encoding.hpp"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-conversion"
#include <boost/regex/pending/unicode_iterator.hpp>
#pragma GCC diagnostic pop

using namespace VA;

Generator<char32_t> VA::u8_to_u32_generator(const std::string& utf8) {
    boost::u8_to_u32_iterator it(utf8.begin());
    boost::u8_to_u32_iterator tend(utf8.end());
    for (; it != tend; ++it) {
        co_yield *it;
    }
}

Generator<char> VA::u32_to_u8_generator(const std::u32string& utf32) {
    boost::u32_to_u8_iterator it(utf32.begin());
    boost::u32_to_u8_iterator tend(utf32.end());
    for (; it != tend; ++it) {
        co_yield (char)*it;
    }
}

std::u32string VA::u8_to_u32_string(const std::string& utf8) {
    // std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> wconv;
    // return wconv.from_bytes(utf8);
    std::u32string result;
    result.reserve(nchars32(utf8));
    for (auto c : u8_to_u32_generator(utf8)) {
        result += c;
    }
    return result;
}

std::string VA::u32_to_u8_string(const std::u32string& wstr) {
    // std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> wconv;
    // return wconv.to_bytes(wstr);
    size_t n = 0;
    for ([[maybe_unused]] auto c : u32_to_u8_generator(wstr)) {
        ++n;
    }
    std::string result;
    result.reserve(n);
    for (auto c : u32_to_u8_generator(wstr)) {
        result += c;
    }
    return result;
}

size_t VA::nchars32(const std::string& utf8) {
    size_t n = 0;
    for ([[maybe_unused]] auto c : u8_to_u32_generator(utf8)) {
        ++n;
    }
    return n;
}

std::u32string VA::ascii_to_u32_string(const std::string& ascii) {
    return { ascii.begin(), ascii.end() };
}

std::string VA::u32_to_ascii_string(const std::u32string& wstr) {
    std::string result(wstr.length(), '?');
    for (size_t i = 0; i < wstr.length(); ++i) {
        unsigned char c = (unsigned char)wstr[i];
        if (c == wstr[i]) {
            result[i] = (char)c;
        }
    }
    return result;
}
