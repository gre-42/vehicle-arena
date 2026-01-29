// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Math/Fixed_Point_Number.hpp>
#include <complex>

namespace VA {

inline constexpr std::complex<float>  conju(const std::complex<float>& v) { return std::conj(v); }
inline constexpr std::complex<double> conju(const std::complex<double>& v) { return std::conj(v); }
inline constexpr std::complex<long double> conju(const std::complex<long double>& v) { return std::conj(v); }
inline constexpr float  conju(const float& v) { return v; }
inline constexpr double conju(const double& v) { return v; }
inline constexpr long double conju(const long double& v) { return v; }
template <class TInt, std::intmax_t denominator>
constexpr FixedPointNumber<TInt, denominator> conju(const FixedPointNumber<TInt, denominator>& v) {
    return v;
}

inline bool is_finite(unsigned char v) { return true; }
inline bool is_finite(const float& v) { return std::isfinite(v); }
inline bool is_finite(const double& v) { return std::isfinite(v); }
inline bool is_finite(const long double& v) { return std::isfinite(v); }
inline bool is_finite(const std::complex<float>& v) { return std::isfinite(v.real()) && std::isfinite(v.imag()) ; }
inline bool is_finite(const std::complex<double>& v) { return std::isfinite(v.real()) && std::isfinite(v.imag()) ; }
inline bool is_finite(const std::complex<long double>& v) { return std::isfinite(v.real()) && std::isfinite(v.imag()) ; }
template <class TInt, std::intmax_t denominator>
constexpr bool is_finite(const FixedPointNumber<TInt, denominator>& v) {
    return true;
}

}
