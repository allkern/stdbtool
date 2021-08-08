#include <filesystem>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstring>
#include <string>
#include <vector>
#include <cmath>

#include "structs.hpp"

namespace stdb {
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
        soundtrack[id].magic = 0x00021371;
        soundtrack[id].id = id;
        //soundtrack[id].total_time_ms = 0; // Retrieve this info from WMA files

        // Set name
        std::memcpy(&soundtrack[id].name[0], &name[0], name.size() * sizeof(wchar_t));

        // Create soundtrack directory
        std::string dir = "fffe0000/music/" + get_zero_padded_string(id, 4);

        std::filesystem::create_directory(dir);

        // If a folder name wasn't specified, use the soundtrack
        // name instead
        if (!folder.size())
            folder = std::filesystem::path(name).string();

        // Create groups based on the amount of songs in this soundtrack
        std::vector <group_t> st_groups;

        int count = 0;

        for (const auto& entry : std::filesystem::directory_iterator(folder))
            count++;

        // Create ceil(n/6) groups to allocate for non multiple of 6
        // song counts
        st_groups.resize(std::ceil((double)count / 6));

        // numSongGroups as described in xboxdevwiki actually
        // contains the amount of tracks
        soundtrack[id].num_songs = count;

        for (group_t& g : st_groups) {
            soundtrack[id].song_group_ids[group_id] = group_id;
            g.id = group_id++;
            g.soundtrack_id = id;
        }

        // Copy songs to the final filesystem and rename them accordingly
        for (const auto& song : std::filesystem::directory_iterator(folder)) {
            std::string zpsid = get_zero_padded_string(song_id++, 8);
            std::filesystem::copy(song.path(), dir + "/" + zpsid + ".wma");

            int idx = std::ceil((double)song_id / 6) - 1,
                group_idx = (song_id - 1) % 6;

            group_t& g = st_groups.at(idx);

            // Set song ID (BCD-like?)
            g.song_id[group_idx] = std::stoi(zpsid, nullptr, 16);

            // Copy song name
            std::wstring song_name = L"Track " + std::to_wstring(song_id);

            std::memcpy(&g.song_name[group_idx * 32], song_name.c_str(), song_name.size() * sizeof(wchar_t));
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