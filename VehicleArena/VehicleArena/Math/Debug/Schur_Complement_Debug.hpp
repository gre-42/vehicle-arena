// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Array/Array_Forward.hpp>
#include <cstddef>

namespace VA { namespace Debug {

void schur_complement_system(
    const Array<float>& H_aa,
    const Array<float>& H_ab,
    const Array<float>& H_ba,
    const Array<float>& H_bb,
    const Array<float>& bp_a,
    const Array<float>& bp_b,
    Array<float>& lhs,
    Array<float>& rhs,
    float alpha,
    float beta);

void schur_complement_jacobian_system(
    const SparseArrayCcs<float>& J,
    const Array<float>& residual,
    const Array<float>& x0,
    const Array<size_t> ids_k,
    const Array<size_t> ids_a,
    const Array<size_t> ids_b,
    Array<float>& lhs_ka,
    Array<float>& rhs_ka,
    Array<size_t>& ids_ka,
    float alpha,
    float beta);

}}
