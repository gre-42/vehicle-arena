// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <iosfwd>
#include <queue>

namespace Mlib {

template <class TData, class TScore>
struct DataAndScore {
    TData data;
    TScore score;
    bool operator < (const DataAndScore& other) const {
        return score < other.score;
    }
    bool operator > (const DataAndScore& other) const {
        return score > other.score;
    }
    friend std::ostream& operator << (std::ostream& ostr, const DataAndScore& element) {
        return ostr << '(' << element.data << ", " << element.score << ')';
    }
};

template <class TData, class TScore, template<class> class TCompare>
class OnlineNthElement:
    private std::priority_queue<
        DataAndScore<TData, TScore>,
        std::vector<DataAndScore<TData, TScore>>,
        TCompare<typename std::vector<DataAndScore<TData, TScore>>::value_type>>
{
    using Q = std::priority_queue<
        DataAndScore<TData, TScore>,
        std::vector<DataAndScore<TData, TScore>>,
        TCompare<typename std::vector<DataAndScore<TData, TScore>>::value_type>>;
public:
    explicit OnlineNthElement(size_t max_size)
        : max_size_{ max_size }
    {
        Q::c.reserve(max_size);
    }
    void insert(const TScore& score, const TData& data) {
        Q& queue = *this;
        if (queue.size() < max_size_) {
            // do nothing
        } else if (TCompare<TScore>()(score, queue.top().score)) {
            queue.pop();
        } else {
            return;
        }
        queue.emplace(data, score);
    }
    size_t size() const {
        const Q& queue = *this;
        return queue.size();
    }
    const DataAndScore<TData, TScore>& nth() const {
        const Q& queue = *this;
        return queue.top();
    }
private:
    size_t max_size_;
};

template <class TData, class TScore>
using OnlineNthSmallest = OnlineNthElement<TData, TScore, std::less>;

template <class TData, class TScore>
using OnlineNthLargest = OnlineNthElement<TData, TScore, std::greater>;

}
