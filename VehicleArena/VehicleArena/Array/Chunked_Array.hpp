// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Os/Os.hpp>
#include <optional>

namespace VA {

template <class TContainer>
class ChunkedArray;

template <class TOuterIterator>
class UnNestedIterator {
public:
    using value_type = std::remove_reference_t<decltype(*TOuterIterator()->begin())>;
    UnNestedIterator(
        const TOuterIterator& outer_begin,
        const TOuterIterator& outer_end)
        : outer_{ outer_begin }
        , outer_end_{ outer_end }
        , inner_{ outer_begin == outer_end
            ? std::nullopt
            : std::optional{ outer_begin->begin() } }
    {}
    ~UnNestedIterator() = default;
    UnNestedIterator& operator ++ () {
        ++(*inner_);
        if (*inner_ == outer_->end()) {
            ++outer_;
            if (outer_ != outer_end_) {
                inner_ = outer_->begin();
            }
        }
        return *this;
    }
    bool operator == (const UnNestedIterator& other) const {
        if (other.inner_.has_value()) {
            verbose_abort("Expected right hand side to be the end iterator");
        }
        return outer_ == other.outer_;
    }
    bool operator != (const UnNestedIterator& other) const {
        return !(*this == other);
    }
    value_type& operator * () {
        return **inner_;
    }
    value_type& operator -> () {
        return **inner_;
    }
private:
    TOuterIterator outer_;
    TOuterIterator outer_end_;
    std::optional<decltype(TOuterIterator()->begin())> inner_;
};

template <class TContainer>
class ChunkedArray {
public:
    using value_type = TContainer::value_type::value_type;
    explicit ChunkedArray(size_t chunk_size)
        : chunk_size_{ chunk_size }
    {}
    ~ChunkedArray() = default;
    template <class... Args>
    inline value_type& emplace_back(Args... args) {
        auto& last = (container_.empty() || (container_.back().size() == container_.back().capacity()))
            ? create_new_chunk()
            : container_.back();
        return last.emplace_back(std::forward<Args>(args)...);
    }
    auto begin() {
        return UnNestedIterator{ container_.begin(), container_.end() };
    }
    auto end() {
        return UnNestedIterator{ container_.end(), container_.end() };
    }
    auto begin() const {
        return UnNestedIterator{ container_.begin(), container_.end() };
    }
    auto end() const {
        return UnNestedIterator{ container_.end(), container_.end() };
    }
private:
    auto& create_new_chunk() {
        auto& res = container_.emplace_back();
        res.reserve(chunk_size_);
        return res;
    }
    TContainer container_;
    size_t chunk_size_;
};

}
