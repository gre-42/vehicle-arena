// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Schur_Complement.hpp"
#include <VehicleArena/Array/Sparse_Array.hpp>
#include <VehicleArena/Math/Math.hpp>

using namespace Mlib;

SchurComplement::SchurComplement(
    const Array<float>& H,
    const Array<float>& b,
    const Array<size_t>& ids_a,
    const Array<size_t>& ids_b)
: H_aa_(H.blocked(ids_a, ids_a)),
  H_bb_(H.blocked(ids_b, ids_b)),
  H_ab_(H.blocked(ids_a, ids_b)),
  H_ba_(H.blocked(ids_b, ids_a)),
  ba_(b.blocked(ids_a)),
  bb_(b.blocked(ids_b)),
  ids_a_(ids_a),
  ids_b_(ids_b),
  shape_(H.shape())
{}

Array<float> SchurComplement::lhs(float alpha, float beta) {
    return H_aa_ - dot(
        H_ab_.casted<double>(),
        solve_symm(
            H_bb_.casted<double>(),
            H_ba_.casted<double>(),
            double(alpha),
            double(beta)).value()).casted<float>();
}

Array<float> SchurComplement::rhs(float alpha, float beta) {
    return ba_.casted<float>() - dot(
        H_ab_.casted<double>(),
        solve_symm_1d(
            H_bb_.casted<double>(),
            bb_.casted<double>(),
            double(alpha),
            double(beta)).value()).casted<float>();
}

Array<float> SchurComplement::solve(float alpha, float beta) {
    Array<float> x;
    Array<float> y;
    solve(x, y, alpha, beta);
    return x.unblocked(ids_a_, shape_(1), 0) + y.unblocked(ids_b_, shape_(1), 0);
}

void SchurComplement::solve(Array<float>& x, Array<float>& y, float alpha, float beta) {
    x = lstsq_chol_1d(
        lhs(alpha, beta),
        rhs(alpha, beta),
        alpha,
        beta).value();
    y = lstsq_chol_1d(
        H_bb_,
        bb_ - dot1d(H_ba_, x),
        alpha,
        beta).value();
}

void ::VA::schur_complement_system(
    const Array<float>& H,
    const Array<float>& bp,
    const Array<size_t>& ids_a,
    const Array<size_t>& ids_b,
    Array<float>& lhs,
    Array<float>& rhs,
    float alpha,
    float beta)
{
    SchurComplement sc{H, bp, ids_a, ids_b};
    lhs = sc.lhs(alpha, beta);
    rhs = sc.rhs(alpha, beta);
}

void ::VA::merge_linear_systems(
    const Array<float>& H,
    const Array<float>& lhs_a,
    const Array<size_t>& ids_k,
    const Array<size_t>& ids_a,
    const Array<float>& bp_k,
    const Array<float>& rhs_a,
    Array<float>& lhs_ka,
    Array<float>& rhs_ka)
{
    lhs_ka.resize(H.shape());
    lhs_ka = 0;
    lhs_ka += H.blocked(ids_k, ids_k).unblocked(ids_k, ids_k, H.shape(), 0);
    lhs_ka += H.blocked(ids_k, ids_a).unblocked(ids_k, ids_a, H.shape(), 0);
    lhs_ka += H.blocked(ids_a, ids_k).unblocked(ids_a, ids_k, H.shape(), 0);
    lhs_ka += lhs_a.unblocked(ids_a, ids_a, H.shape(), 0);
    // lhs_ka += H.blocked(ids_a, ids_a).unblocked(ids_a, ids_a, H.shape(), 0);

    rhs_ka.resize(ArrayShape{H.shape(1)});
    rhs_ka = 0;
    rhs_ka += bp_k.unblocked(ids_k, H.shape(0), 0);
    rhs_ka += rhs_a.unblocked(ids_a, H.shape(0), 0);
}

void ::VA::marginalize(
    const Array<float>& H,
    const Array<float>& bp,
    const Array<size_t>& ids_k,
    const Array<size_t>& ids_a,
    const Array<size_t>& ids_b,
    Array<float>& lhs_ka,
    Array<float>& rhs_ka,
    float alpha,
    float beta)
{
    Array<float> bp_k = bp.blocked(ids_k);

    Array<float> lhs_a;
    Array<float> rhs_a;

    schur_complement_system(H, bp, ids_a, ids_b, lhs_a, rhs_a, alpha, beta);
    merge_linear_systems(H, lhs_a, ids_k, ids_a, bp_k, rhs_a, lhs_ka, rhs_ka);
}

void ::VA::marginalize_least_squares(
    const SparseArrayCcs<float>& J,
    const Array<float>& residual,
    const Array<float>& x0,
    const Array<size_t>& ids_k,
    const Array<size_t>& ids_a,
    const Array<size_t>& ids_b,
    Array<float>& lhs_ka,
    Array<float>& rhs_ka,
    float alpha,
    float beta)
{
    Array<float> H = dot2d(J.vH(), J);
    Array<float> bp = dot1d(J.vH(), residual) + dot1d(H, x0);

    marginalize(H, bp, ids_k, ids_a, ids_b, lhs_ka, rhs_ka, alpha, beta);
}
