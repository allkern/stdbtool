#ifndef __STDB_HPP
#define __STDB_HPP

#include <string>
#include <vector>

class stdb {
private:
    struct header_t {
        int32_t magic = 0x1,
            num_soundtracks,
            next_soundtrack_id,
            soundtrack_id[100],
            next_song_id;

        char pad[96];
    } header;

    struct soundtrack_t {
        int32_t magic, // Only fill in magic number when a soundtrack is actually generated
            id,
            num_songs,
            song_group_ids[84],
            total_time_ms;

        // Hack? Not sure how the soundtrack name is stored.
        // I've found that these always (?) start at offset
        // +0x160, a padding of this size will accomplish
        // this.
        wchar_t name[80];
    } soundtrack[100];

    struct group_t {
        int32_t magic = 0x00031073,
            soundtrack_id,
            id,
            padding = 0x1,
            song_id[6],
            song_time_ms[6];
        wchar_t song_name[192];
        
        char pad[64];
    };

    std::vector <group_t> groups;
    int group_id = 0;
    int song_id = 0;

public:
    stdb();
    void create_soundtrack(std::wstring name, std::string folder = "");
    void write();

    std::string get_zero_padded_string(int n, int w = 4);
};

#endif
