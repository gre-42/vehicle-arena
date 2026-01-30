// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <iosfwd>
#include <string>
#include <string_view>

namespace VA {

class GroupAndName {
public:
    GroupAndName()
        : group_length_{ 0 }
    {}
    GroupAndName(const char* full_name)
        : full_name_{ full_name }
        , group_length_{ 0 }
    {}
    GroupAndName(std::string full_name)
        : full_name_{ std::move(full_name) }
        , group_length_{ 0 }
    {}
    GroupAndName(const std::string& group, const std::string& name)
        : full_name_{ group + name }
        , group_length_{ group.length() }
    {}
    inline const std::string& full_name() const {
        return full_name_;
    }
    inline std::string_view group() const {
        return std::string_view{ full_name_.data(), group_length_ };
    }
    inline std::string_view name() const {
        return std::string_view{
            full_name_.data() + group_length_,
            full_name_.length() - group_length_
        };
    }
    inline bool empty() const {
        return full_name_.empty();
    }
    GroupAndName operator + (const std::string& rhs) const {
        return { full_name() + rhs, group_length_ };
    }
    template <class Archive>
    void serialize(Archive& archive) {
        archive(full_name_);
    }
private:
    GroupAndName(std::string full_name, size_t group_length)
        : full_name_{ std::move(full_name) }
        , group_length_{ group_length }
    {}
    std::string full_name_;
    size_t group_length_;
};

inline std::ostream& operator << (std::ostream& ostr, const GroupAndName& n) {
    return (ostr << n.full_name());
}

}
