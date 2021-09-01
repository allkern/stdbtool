#pragma once

#include <filesystem>
#include <vector>
#include <string>

#include "log.hpp"

namespace cli {
    std::vector <std::string> args, input, output;

    bool output_stdb_only = false,
         auto_input = false,
         cleanup = false;

    void init(int argc, const char* argv[]) {
        int idx = 0;

        args.resize(argc);

        while (idx < argc)
            args[idx++] = std::string(argv[idx]);

        args.erase(std::begin(args));
    }

    void parse() {
        // Check for -a and -s switches
        auto c = std::find(std::begin(args), std::end(args), "-c");

        if (c != std::end(args)) { cleanup = true; args.erase(c); return; }

        auto s = std::find(std::begin(args), std::end(args), "-s");

        if (s != std::end(args)) { output_stdb_only = true; args.erase(s); }

        auto a = std::find(std::begin(args), std::end(args), "-a");

        if (a != std::end(args)) { auto_input = true; args.erase(a); }

        std::string input_lookup_path;

        size_t output_idx = std::distance(std::begin(args), std::find(std::begin(args), std::end(args), "-o")) + 1,
               input_idx = std::distance(std::begin(args), std::find(std::begin(args), std::end(args), "-i"));

        if (input_idx != args.size()) {
            input_lookup_path = args[input_idx + 1];
        } else {
            input_lookup_path = ".";
        }

        if (output_idx != args.size() + 1) {
            while (output_idx < args.size()) {
                if ((args[output_idx] == "-s") || (args[output_idx] == "-a") || (args[output_idx] == "-i")) break;

                output.push_back(args[output_idx]);

                args.erase(std::begin(args) + output_idx);
            }


            args.erase(std::begin(args) + (output_idx - 1));
        }

        if (!auto_input) {
            for (auto s : args) input.push_back(s);

            return;
        }

        // Enumerate music folders on this directory
        for (const auto& entry : std::filesystem::directory_iterator(input_lookup_path)) {
            if (entry.is_directory() && (entry.path().filename() != "fffe0000"))
                input.push_back(input_lookup_path + "/" + entry.path().filename().string());
        }
    }
}