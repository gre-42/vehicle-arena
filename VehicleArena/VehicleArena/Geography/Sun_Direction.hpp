// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <chrono>
#include <cstddef>

namespace VA {

// From: https://github.com/sczesla/PyAstronomy/blob/master/src/pyasl/asl/eq2hor.py

template <typename TData, size_t... tshape>
class FixedArray;
enum class Season;

template <typename TData>
TData days_since_noon_2000_1_1(const std::chrono::system_clock::time_point& time);

template <typename TData>
void sun_angles(
    double jddays,
    TData& ra,
    TData& dec);

template <class TData>
FixedArray<TData, 3> sun_direction(
    const std::chrono::system_clock::time_point& time,
    TData latitude,
    TData longitude);

template <class TData>
std::chrono::system_clock::time_point time_of_season(
    Season season,
    const std::chrono::system_clock::time_point& start_time,
    TData latitude,
    TData longitude);

}
