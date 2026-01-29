// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <Mlib/Math/Math.hpp>

namespace Mlib {

template <class TData>
std::pair<TData, TData> linspace_multipliers(size_t i, size_t count) {
    return std::make_pair(
        TData(count - i - 1) / (TData)(count - 1),
        TData(i) / (TData)(count - 1));
}

template <class TData>
class LinspaceIterator;

template <class TData>
class Linspace {
public:
    Linspace(const TData& from, const TData& to, size_t count)
    : from_{from}, to_{to}, count_{count}
    {}
    TData operator [] (size_t i) const {
        if (count_ == 0) {
            THROW_OR_ABORT("Linspace called, but count == 0");
        }
        if (count_ == 1) {
            return (from_ + to_) / TData(2);
        }
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
        return (from_ * TData(count_ - i - 1) + to_ * TData(i)) / TData(count_ - 1);
#pragma GCC diagnostic pop
    }
    size_t length() const {
        return count_;
    }
    LinspaceIterator<TData> begin() const {
        return LinspaceIterator<TData>{*this, 0};
    }
    LinspaceIterator<TData> end() const {
        return LinspaceIterator<TData>{*this, count_};
    }
private:
    TData from_;
    TData to_;
    size_t count_;
};

template <class TData>
class LinspaceIterator {
public:
    LinspaceIterator(const Linspace<TData>& linspace, size_t i)
    : linspace_{linspace},
      i_{i}
    {}
    TData operator * () const {
        return linspace_[i_];
    }
    LinspaceIterator& operator ++ () {
        ++i_;
        return *this;
    }
    bool operator != (const LinspaceIterator& other) const {
        return i_ != other.i_;
    }
private:
    const Linspace<TData>& linspace_;
    size_t i_;
};

template <class TFloat>
Array<std::pair<TFloat, TFloat>> linspace_multipliers(size_t count) {
    Array<std::pair<TFloat, TFloat>> result{ArrayShape{count}};
    if (count == 1) {
        result = std::make_pair(1 / TFloat(2), 1 / TFloat(2));
    } else {
        for (size_t i = 0; i < count; ++i) {
            result(i) = linspace_multipliers<TFloat>(i, count);
        }
    }
    return result;
}

template <class TData>
Array<TData> linspace(const TData& from, const TData& to, size_t count) {
    Array<TData> result{ArrayShape{count}};
    if (count == 1) {
        result = (from + to) / 2;
    } else {
        for (size_t i = 0; i < count; ++i) {
            result(i) = (from * TData(count - i - 1) + to * TData(i)) / TData(count - 1);
        }
    }
    return result;
}

}
