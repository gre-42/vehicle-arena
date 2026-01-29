// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <cstddef>

namespace VA {

template <class TData>
class Array;

template <class TData>
class ConjugateTransposeArray;

template <typename TData, size_t... tshape>
class FixedArray;

namespace CW {

// nelements
template <class T>
size_t nelements(const Array<T>& a) {
    return a.nelements();
}

template <class T, size_t... tshape>
constexpr size_t nelements(const FixedArray<T, tshape...>&) {
    return FixedArray<T, tshape...>::nelements();
}

// length
template <class T>
size_t length(const Array<T>& a) {
    return a.length();
}

template <class T, size_t... tshape>
constexpr size_t length(const FixedArray<T, tshape...>&) {
    return FixedArray<T, tshape...>::length();
}

// ndim
template <class T>
size_t ndim(const Array<T>& a) {
    return a.ndim();
}

template <class T>
size_t ndim(const ConjugateTransposeArray<T>& a) {
    return a.ndim();
}

template <class T, size_t... tshape>
constexpr size_t ndim(const FixedArray<T, tshape...>&) {
    return FixedArray<T, tshape...>::ndim();
}

// static_shape
template <size_t N, class T>
size_t static_shape(const Array<T>& a) {
    return a.shape(N);
}

template <size_t N, class T>
size_t static_shape(const ConjugateTransposeArray<T>& a) {
    return a.shape(N);
}

template <size_t N, class T, size_t... tshape>
constexpr size_t static_shape(const FixedArray<T, tshape...>&) {
    return FixedArray<T, tshape...>::template static_shape<N>();
}

}

}
