// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <iosfwd>

#ifdef __clang__

namespace VA {

class SourceLocation {
public:
    inline SourceLocation(const char* file_name, int line)
    : file_name_{file_name},
      line_{line}
    {}
    inline const char* file_name() const {
        return file_name_;
    }
    inline int line() const {
        return line_;
    }
private:
    const char* file_name_;
    int line_;
};
#define CURRENT_SOURCE_LOCATION SourceLocation{__FILE__, __LINE__}

}

#else

#include <source_location>

namespace VA {

using SourceLocation = std::source_location;
#define CURRENT_SOURCE_LOCATION std::source_location::current()

}

#endif

namespace VA {

std::ostream& operator << (std::ostream& ostr, const SourceLocation& loc);

}
