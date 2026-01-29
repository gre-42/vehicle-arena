// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <Mlib/Strings/To_Number.hpp>
#include <cmath>
#include <cstdint>
#include <iosfwd>
#include <istream>
#include <string>
#include <type_traits>

namespace VA {

template <class TNum>
std::istream& read_num(std::istream& istr, TNum& num) {
    if constexpr (std::is_floating_point_v<TNum>) {
        std::string str;
        istr >> str;
        if (!istr.fail() || (istr.eof() && !str.empty())) {
            num = safe_sto<TNum>(str);
        }
        return istr;
    } else {
        return (istr >> num);
    }
}

template <class TNum>
class ReadNum {
    template <class TNum2>
    friend std::istream& operator >> (std::istream& istr, const ReadNum<TNum2>& num);
public:
    ReadNum(TNum& num)
    : num_{num}
    {}
private:
    TNum& num_;
};

template <class TNum>
std::istream& operator >> (std::istream& istr, const ReadNum<TNum>& num) {
    return read_num(istr, num.num_);
}

}
