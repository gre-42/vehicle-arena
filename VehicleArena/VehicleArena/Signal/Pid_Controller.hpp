// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Signal/Exponential_Smoother.hpp>
#include <optional>
#include <ostream>

namespace VA {

template <class TData, class TFloat>
class PidController {
    template <class TData2, class TFloat2>
    friend std::ostream& operator << (std::ostream& ostr, const PidController<TData2, TFloat2>& pid);
public:
    PidController(
        const TFloat& p,
        const TFloat& i,
        const TFloat& d,
        const TFloat& a,
        const TData& I = (TData)0,
        const std::optional<TData>& e_old = std::nullopt)
        : p_{ p }
        , i_{ i }
        , d_{ d }
        , I_{ a, I }
        , e_old_{ e_old }
    {}
    PidController(
        const TFloat& p,
        const TFloat& i,
        const TFloat& d,
        const ExponentialSmoother<TData, TFloat>& I,
        const std::optional<TData>& e_old)
        : p_{ p }
        , i_{ i }
        , d_{ d }
        , I_{ I }
        , e_old_{ e_old }
    {}
    TData operator () (const TData& e) {
        TData result = e_old_.has_value()
            ? p_ * e + i_ * I_(e) + d_ * (e - *e_old_)
            : p_ * e + i_ * I_(e);
        e_old_ = e;
        return result;
    }
    TData operator () (const TData& e, const TFloat& from, const TFloat& to) {
        auto cts = changed_time_step(from, to);
        auto result = cts(e);
        I_.set(*cts.I_.xhat());
        e_old_ = e;
        return result;
    }
    PidController changed_time_step(const TFloat& from, const TFloat& to) const {
        // The factors were determined by trial and error in the test "test_pid".
        auto f = from / to;
        return {
            p_,
            i_,
            d_ * f,
            I_.changed_time_step(from, to),
            e_old_ };
    }
private:
    TFloat p_;
    TFloat i_;
    TFloat d_;
    ExponentialSmoother<TData, TFloat> I_;
    std::optional<TData> e_old_;
};

template <class TData, class TFloat>
inline std::ostream& operator << (std::ostream& ostr, const PidController<TData, TFloat>& pid) {
    ostr <<
        "p=" << pid.p_ <<
        " i=" << pid.i_ <<
        " d=" << pid.d_ <<
        " a=" << pid.a_ <<
        " I=" << pid.I_;
    if (pid.e_old_.has_value()) {
        ostr << " e_old=" << *pid.e_old_ ;
    } else {
        ostr << " e_old=null";
    }
    return ostr;
}

}
