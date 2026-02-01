// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <cmath>

namespace VA {

template <class TData>
class KalmanFilter {
public:
    /**
     * Kalman filter

     * Parameters
     * ----------
     * ziter : iterable
     *     Observations ~ 𝓝(x, √R).
     * xhat : TData, optional
     *     Initial estimate for x.
     * Q : TData
     *     Process variance.
     * R : TData
     *     Measurement variance.
     * P : TData
     *     Posteriori error variance.
     *
     * References
     * ----------
     *     http://wiki.scipy.org/Cookbook/KalmanFiltering
     */
    explicit KalmanFilter(const TData& Q=1e-5, const TData& R=1e-2, const TData& P=1, const TData& xhat=NAN)
    : Q_{Q},
      R_{R},
      P_{P},
      xhat_{xhat}
    {}
    const TData& operator () (const TData& z) {
        if (std::isnan(xhat_)) {
            xhat_ = z;  // a posteri estimate of x
        } else {
            // time update
            TData xhatminus = xhat_;  // a priori estimate of x
            TData Pminus = P_ + Q_;   // a priori error estimate

            // measurement update
            TData K = Pminus / (Pminus + R_);  // gain or blending factor
            xhat_ = xhatminus + K * (z - xhatminus);
            P_ = (1 - K) * Pminus;
        }
        return xhat_;
    }
    const TData& xhat() const {
        return xhat_;
    }
private:
    TData Q_;
    TData R_;
    TData P_;
    TData xhat_;
};

}
