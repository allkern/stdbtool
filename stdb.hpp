#include <filesystem>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstring>
#include <string>
#include <vector>
#include <cmath>

namespace stdb {
    struct header_t {
        int32_t magic = 0x1,
                num_soundtracks,
                next_soundtrack_id,
                soundtrack_id[100],
                next_song_id;

        char pad[96];
    } header;

    struct soundtrack_t {
        int32_t magic = 0x00021371,
                id,
                num_song_groups,
                song_group_ids,
                total_time_ms;

        // Hack? Not sure how the soundtrack name is stored.
        // I've found that these always (?) start at offset
        // +0x160, a padding of this size will accomplish
        // this.
        char pad[0x14a];

        wchar_t name[80];
    } soundtrack[100];

    struct group_t {
        int32_t magic = 0x00031073,
                soundtrack_id,
                id,
                padding,
                song_id[6],
                song_time_ms[6];
        wchar_t song_name[384];
        
        char pad[64];
    };

    std::vector <group_t> groups;

    int group_id = 0;

    std::string get_zero_padded_string(int n, int w = 4) {
        std::ostringstream ss;

        ss << std::dec << std::setw(w) << std::setfill('0') << n;

        return ss.str();
    }

    bool init() {
        if (std::filesystem::exists("fffe0000"))
            std::filesystem::remove_all("fffe0000");

        bool r = std::filesystem::create_directory("fffe0000");

        return r && std::filesystem::create_directory("fffe0000/music");
    }

    int song_id = 0;

    void create_soundtrack(std::wstring name, std::string folder = "") {
        // Get current soundtrack ID and increment
        int32_t id = header.num_soundtracks++;

        // Fill in ID in header
        header.soundtrack_id[id] = id;

        // Fill in soundtrack info
        soundtrack[id].id = id;
        //soundtrack[id].total_time_ms = 0; // Retrieve this info from WMA files
        //soundtrack[id].song_group_ids = 0; // Not needed?

        // Set name
        int idx = 0;

        while (name[idx] != L'\0')
            soundtrack[id].name[idx] = name[idx++];

        // Create soundtrack directory
        std::string dir = "fffe0000/music/" + get_zero_padded_string(id, 4);

        std::filesystem::create_directory(dir);

        // If a folder name wasn't specified, use the soundtrack
        // name instead
        if (!folder.size())
            folder = std::filesystem::path(name).string();

        // Create groups based on amount of songs in this soundtrack
        std::vector <group_t> st_groups;
        int count = 0;

        for (const auto& entry : std::filesystem::directory_iterator(folder))
            count++;

        // Create ceil(n/6) groups to allocate for non multiple of 6
        // song counts
        st_groups.resize(std::ceil((double)count / 6));

        soundtrack[id].num_song_groups = st_groups.size();

        for (group_t& g : st_groups) {
            g.id = group_id++;
            g.soundtrack_id = id;
        }

        // Copy songs to the final filesystem and rename them accordingly
        for (const auto& song : std::filesystem::directory_iterator(folder)) {
            std::filesystem::copy(song.path(), dir + "/" + get_zero_padded_string(song_id++, 8) + ".wma");

            int idx = std::ceil((double)song_id / 6) - 1;

            group_t& g = st_groups.at(idx);

            g.song_id[(song_id - 1) % 6] = song_id - 1;

            std::wstring song_name = L"Track " + std::to_wstring(song_id);

            std::memcpy(&g.song_name[((song_id - 1) % 6) * 32], song_name.c_str(), song_name.size() * sizeof(wchar_t));
        }

        for (group_t& g : st_groups)
            groups.push_back(g);
    }

    void write() {
        std::ofstream f("fffe0000/music/ST.DB", std::ios::binary);

        f.write((char*)&header, sizeof(header_t));

        for (int i = 0; i < 100; i++)
            f.write((char*)&soundtrack[i], sizeof(soundtrack_t));

        f.write((char*)groups.data(), groups.size() * sizeof(group_t));
    }
}