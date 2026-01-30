// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Trim.hpp"
#include <algorithm> 
#include <cctype>
#include <locale>

using namespace VA;

// From: https://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring

// Trim spaces

void VA::ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
}

void VA::rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

void VA::trim(std::string &s) {
    ltrim(s);
    rtrim(s);
}

std::string VA::ltrim_copy(std::string s) {
    ltrim(s);
    return s;
}

std::string VA::rtrim_copy(std::string s) {
    rtrim(s);
    return s;
}

std::string VA::trim_copy(std::string s) {
    trim(s);
    return s;
}

// Trim spaces

void VA::ltrim(std::string &s, char c) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [c](unsigned char ch) {
        return ch != c;
    }));
}

void VA::rtrim(std::string &s, char c) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [c](unsigned char ch) {
        return ch != c;
    }).base(), s.end());
}

void VA::trim(std::string &s, char c) {
    ltrim(s, c);
    rtrim(s, c);
}

std::string VA::ltrim_copy(std::string s, char c) {
    ltrim(s, c);
    return s;
}

std::string VA::rtrim_copy(std::string s, char c) {
    rtrim(s, c);
    return s;
}

std::string VA::trim_copy(std::string s, char c) {
    trim(s, c);
    return s;
}
