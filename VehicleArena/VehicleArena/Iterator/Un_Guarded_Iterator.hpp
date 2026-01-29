// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <utility>

namespace VA {

template<typename Iterable, typename TLock>
class UnGuardedIterator
{
private:
    static Iterable container();
    TLock& lock_;
    decltype(std::begin(container())) begin_;
    decltype(std::end(container())) end_;

    UnGuardedIterator(const UnGuardedIterator&) = delete;
    UnGuardedIterator& operator = (const UnGuardedIterator&) = delete;
public:
    struct end_type {};
    UnGuardedIterator(Iterable& iter, TLock& lock)
        : lock_{ lock }
        , begin_(std::begin(iter))
        , end_(std::end(iter))
    {
        lock_.unlock();
    }

    ~UnGuardedIterator() {
        lock_.lock();
    }

    bool operator != (const end_type&) const
    {
        return begin_ != end_;
    }

    void operator++()
    {
        lock_.lock();
        ++begin_;
        lock_.unlock();
    }

    decltype(*begin_) operator*() const {
        return *begin_;
    }
};

template<typename Iterable, typename TLock>
class UnGuardedIterable
{
private:
    Iterable& container_;
    TLock& lock_;

    UnGuardedIterable(const UnGuardedIterable&) = delete;
    UnGuardedIterable& operator = (const UnGuardedIterable&) = delete;
public:
    struct end_type {};
    UnGuardedIterable(Iterable& iter, TLock& lock)
        : container_{ iter }
        , lock_{ lock }
    {}

    UnGuardedIterator<Iterable, TLock> begin() const { return UnGuardedIterator<Iterable, TLock>{ container_, lock_ }; }
    UnGuardedIterator<Iterable, TLock>::end_type end() const { return {}; }
};

template<typename Iterable, typename TLock>
void un_guarded_iterator(Iterable&& iter, TLock& lock);

template<typename Iterable, typename TLock>
auto un_guarded_iterator(Iterable& iter, TLock& lock) -> UnGuardedIterable<Iterable&, TLock>
{
    return { iter, lock };
}

template<typename Iterable, typename TLock>
auto un_guarded_iterator(const Iterable& iter, TLock& lock) -> UnGuardedIterable<const Iterable&, TLock>
{
    return { iter, lock };
}

}
