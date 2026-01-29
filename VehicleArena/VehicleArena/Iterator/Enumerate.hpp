// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <iterator>
#include <utility>

namespace VA {

// From: https://stackoverflow.com/questions/11328264/python-like-loop-enumeration-in-c
template<class TIndex, typename Iterable>
class enumerate_named_object
{
private:
    static Iterable container();
    TIndex size_;
    decltype(std::begin(container())) begin_;
    decltype(std::end(container())) end_;

public:
    struct end_type {};
    enumerate_named_object(Iterable& iter)
        : size_(0)
        , begin_(std::begin(iter))
        , end_(std::end(iter))
    {}

    const enumerate_named_object& begin() const { return *this; }
    end_type end() const { return {}; }

    bool operator != (const end_type&) const
    {
        return begin_ != end_;
    }

    void operator++()
    {
        ++begin_;
        ++size_;
    }

    std::pair<TIndex, decltype(*begin_)> operator*() const {
        return { size_, *begin_ };
    }
};

template<class TIndex, typename Iterable>
class enumerate_temporary_object
{
private:
    Iterable container_;
    TIndex size_;
    decltype(std::begin(container_)) begin_;
    decltype(std::end(container_)) end_;

public:
    struct end_type {};
    enumerate_temporary_object(enumerate_temporary_object&& other) noexcept
        : enumerate_temporary_object{ std::move(other.container_) }
    {}
    enumerate_temporary_object(Iterable&& iter)
        : container_(std::move(iter))
        , size_(0)
        , begin_(std::begin(container_))
        , end_(std::end(container_))
    {}

    enumerate_temporary_object begin() { return std::move(*this); }
    end_type end() const { return {}; }

    bool operator != (const end_type&) const
    {
        return begin_ != end_;
    }

    void operator++()
    {
        ++begin_;
        ++size_;
    }

    std::pair<TIndex, decltype(*begin_)> operator*() const {
        return { size_, *begin_ };
    }
};

template<class TIndex, typename Iterable>
auto tenumerate(Iterable&& iter) -> enumerate_temporary_object<TIndex, Iterable>
{
    return { std::move(iter) };
}

template<class TIndex, typename Iterable>
auto tenumerate(Iterable& iter) -> enumerate_named_object<TIndex, Iterable&>
{
    return { iter };
}

template<class TIndex, typename Iterable>
auto tenumerate(const Iterable& iter) -> enumerate_named_object<TIndex, const Iterable&>
{
    return { iter };
}

template<typename Iterable>
auto enumerate(Iterable&& iter) {
    return tenumerate<std::size_t>(std::move(iter));
}

template<typename Iterable>
auto enumerate(Iterable& iter) {
    return tenumerate<std::size_t>(iter);
}

template<typename Iterable>
auto enumerate(const Iterable& iter) {
    return tenumerate<std::size_t>(iter);
}

}
