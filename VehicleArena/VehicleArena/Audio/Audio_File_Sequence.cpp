// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Audio_File_Sequence.hpp"
#include <VehicleArena/Json/Misc.hpp>
#include <VehicleArena/Os/Os.hpp>

namespace fs = std::filesystem;
using namespace VA;

namespace VA {

void from_json(const nlohmann::json& j, AudioFileSequenceItem& item) {
    j.at("filename").get_to(item.filename);
    j.at("frequency").get_to(item.frequency);
    j.at("key").get_to(item.key);
}

}

std::vector<AudioFileSequenceItem> VA::load_audio_file_sequence(
    const std::string& filename)
{
    auto ifs = create_ifstream(filename);
    if (ifs->fail()) {
        throw std::runtime_error("Could not open file \"" + filename + '"');
    }
    nlohmann::json j;
    *ifs >> j;
    if (!ifs->eof() && ifs->fail()) {
        throw std::runtime_error("Error reading from file: \"" + filename + '"');
    }
    auto res = j.get<std::vector<AudioFileSequenceItem>>();
    for (auto& i : res) {
        i.filename = (fs::path{filename}.parent_path() / i.filename).string();
    }
    return res;
}
