#include "stdb/stdb.hpp"
#include "cli.hpp"

#include <algorithm>
/*
    Usage: stdbtool <input...> <options>

    Options:
        -s Generate ST.DB file only
        -a Generate soundtracks for all folders in the current directory
        -o Specify soundtrack names for every input folder
*/

int main(int argc, const char* argv[]) {
    _log::init("stdbtool");

    cli::init(argc, argv);
    cli::parse();

    if (cli::cleanup) {
        std::filesystem::remove_all("fffe0000");
        _log(info, "Cleaned up directories");

        return 0;
    };

    stdb::init();

    for (int i = 0; i < cli::input.size(); i++) {
        std::wstring stname;

        if (i < cli::output.size()) {
            stname = std::filesystem::path(cli::output[i]).filename().wstring();
        } else {
            stname = std::filesystem::path(cli::input[i]).filename().wstring();
        }

        _log(debug, "Creating soundtrack \"%ls\" from folder \"%s\"", stname.c_str(), cli::input[i].c_str());
        stdb::create_soundtrack(stname, cli::input[i]);
    }

    size_t filesize = stdb::write();
    _log(debug, "Generated ST.DB (%.02f KB)", (double)filesize / 1000);
    return 0;
}