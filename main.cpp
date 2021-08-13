#include "stdb.hpp"
#include <string>

int main(int argc, char* argv[]) {
    std::wstring name = L"test";
    std::string path = "test";
    if (argc >= 2) {
        name = std::wstring(&argv[1][0], &argv[1][sizeof(argv[1])]);
        path = argv[2];
    }
    stdb::init();
    stdb::create_soundtrack(name, path);
    stdb::write();
}
