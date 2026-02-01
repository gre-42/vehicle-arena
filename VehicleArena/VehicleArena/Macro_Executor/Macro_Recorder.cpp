// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Macro_Recorder.hpp"
#include <VehicleArena/Macro_Executor/Macro_Line_Executor.hpp>
#include <VehicleArena/Macro_Executor/Replacement_Parameter.hpp>
#include <VehicleArena/Misc/Argument_List.hpp>
#include <VehicleArena/Os/Os.hpp>
#include <fstream>
#include <iostream>
#include <stdexcept>

using namespace VA;

MacroRecorder::MacroRecorder() = default;

MacroRecorder::~MacroRecorder() = default;

void MacroRecorder::operator()(const MacroLineExecutor& macro_line_executor)
{
    if (macro_line_executor.script_filename_.ends_with(".scn.json")) {
        std::scoped_lock lock{ include_mutex_ };
        if (included_files_.contains(macro_line_executor.script_filename_)) {
            return;
        }
        if (!included_files_.insert(macro_line_executor.script_filename_).second) {
            verbose_abort("Internal error, could not insert included file");
        }
        auto ifs = create_ifstream(macro_line_executor.script_filename_);
        if (ifs->fail()) {
            throw std::runtime_error("Could not open script file \"" + macro_line_executor.script_filename_ + '"');
        }
        if (macro_line_executor.verbose_) {
            linfo() << "Processing JSON scene file \"" << macro_line_executor.script_filename_ << '"';
        }
        nlohmann::json j;
        try {
            *ifs >> j;
        } catch (const nlohmann::json::exception& e) {
            throw std::runtime_error("Error loading file \"" + macro_line_executor.script_filename_ + "\": " + e.what());
        }
        if (!ifs->eof() && ifs->fail()) {
            throw std::runtime_error("Error reading from file: \"" + macro_line_executor.script_filename_ + '"');
        }
        macro_line_executor(j, nullptr);
    } else if (macro_line_executor.script_filename_.ends_with(".json")) {
        auto rp = ReplacementParameterAndFilename::from_json(macro_line_executor.script_filename_);
        if (macro_line_executor.verbose_) {
            linfo() << "Processing JSON macro \"" << rp.rp.on_execute << '"';
        }
        macro_line_executor(rp.rp.on_execute, nullptr);
    } else {
        throw std::runtime_error("Unknown script file extension: \"" + macro_line_executor.script_filename_ + '"');
    }
}
