#include "stdb.hpp"

int main() {
    stdb::init();
    stdb::create_soundtrack(L"test", "test");
    stdb::write();
}