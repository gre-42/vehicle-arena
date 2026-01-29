// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <Mlib/Throw_Or_Abort.hpp>
#include <fstream>
#include <list>
#include <string>

namespace VA {

std::list<std::string> read_lines_from_file(const std::string& filename) {
    std::list<std::string> result;
    std::ifstream ifs(filename);
    if (ifs.fail()) {
        THROW_OR_ABORT("Could not open " + filename);
    }
    while (ifs.peek() != EOF) {
        std::string line;
        std::getline(ifs, line);
        if (ifs.fail()) {
            THROW_OR_ABORT("Could not read from " + filename);
        }
        result.push_back(line);
    }
    return result;
}

}
