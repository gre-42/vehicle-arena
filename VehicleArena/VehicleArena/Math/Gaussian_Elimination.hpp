// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Memory/Integral_Cast.hpp>

// source: https://rosettacode.org/wiki/Gaussian_elimination#C

#define mat_elem(a, y, x, n) (a + ((y) * (n) + (x)))

template <class TData>
void swap_row(TData *a, TData *b, int r1, int r2, int n)
{
    TData tmp, *p1, *p2;
    int i;

    if (r1 == r2) return;
    for (i = 0; i < n; i++) {
        p1 = mat_elem(a, r1, i, n);
        p2 = mat_elem(a, r2, i, n);
        tmp = *p1, *p1 = *p2, *p2 = tmp;
    }
    tmp = b[r1], b[r1] = b[r2], b[r2] = tmp;
}

template <class TData>
void gauss_eliminate(TData *a, TData *b, TData *x, int n)
{
#define A(y, x) (*mat_elem(a, y, x, n))
    int j, col, row, max_row,dia;
    TData max, tmp;

    for (dia = 0; dia < n; dia++) {
        max_row = dia, max = A(dia, dia);

        for (row = dia + 1; row < n; row++)
            if ((tmp = std::abs(A(row, dia))) > max)
                max_row = row, max = tmp;

        swap_row(a, b, dia, max_row, n);

        for (row = dia + 1; row < n; row++) {
            tmp = A(row, dia) / A(dia, dia);
            for (col = dia+1; col < n; col++)
                A(row, col) -= tmp * A(dia, col);
            A(row, dia) = 0;
            b[row] -= tmp * b[dia];
        }
    }
    for (row = n - 1; row >= 0; row--) {
        tmp = b[row];
        for (j = n - 1; j > row; j--)
            tmp -= x[j] * A(row, j);
        x[row] = tmp / A(row, row);
    }
#undef A
}

#undef mat_elem

#include <VehicleArena/Array/Array.hpp>
#include <VehicleArena/Math/Optimize/Solve_Penalized.hpp>

namespace VA {

template <class TData>
Array<TData> gaussian_elimination_1d(const Array<TData>& a, const Array<TData>& b) {
    assert(a.ndim() == 2);
    assert(a.shape(0) == a.shape(1));
    assert(b.length() == a.shape(0));
    Array<TData> x{b.shape()};
    int n = integral_cast<int>(b.length());
    assert(n >= 0);
    gauss_eliminate(
        a.copy().flat_iterable().begin(),
        b.copy().flat_iterable().begin(),
        x.flat_iterable().begin(),
        n);
    return x;
}

template <class TData>
Array<TData> gaussian_elimination_1d(
    const Array<TData>& a,
    const Array<TData>& b,
    const TData& alpha,
    const TData& beta)
{
    return solve_penalized(
        [](const Array<float>& L, const Array<float>& R){
            return gaussian_elimination_1d(L, R.flattened());
        },
        a,
        b.as_column_vector(),
        alpha,
        beta);
}

}
