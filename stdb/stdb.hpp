#pragma once

#include <filesystem>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstring>
#include <string>
#include <vector>
#include <cmath>

#include "../log.hpp"

#include "structs.hpp"

namespace stdb {
    std::vector <group_t> global_groups;

    int global_group_id = 0;

    std::string get_zero_padded_string(int n, int w = 4) {
        std::ostringstream ss;

        ss << std::dec << std::setw(w) << std::setfill('0') << n;

        return ss.str();
    }

    bool init() {
        if (std::filesystem::exists("fffe0000"))
            std::filesystem::remove_all("fffe0000");

        return std::filesystem::create_directories("fffe0000/music/0000");
    }

    int global_song_id = 0;

    void create_soundtrack(std::wstring name, std::string folder = "", bool create_filesystem = true) {
        // Get current soundtrack ID and increment
        int32_t id = header.num_soundtracks++;

        // Fill in ID in header
        header.soundtrack_id[id] = id;

        // Fill in soundtrack info
        soundtrack[id].magic = STDB_SOUNDTRACK_MAGIC;
        soundtrack[id].id = id;
        //soundtrack[id].total_time_ms = 0; // Retrieve this info from WMA files

        // Copy name to soundtrack
        std::memcpy(&soundtrack[id].name[0], &name[0], name.size() * sizeof(wchar_t));

        // Create soundtrack directory
        //
        // To-do: Check this with different ST.DB configurations:
        // 
        // Apparently, the Xbox will look for songs with IDs only
        // on the first soundtrack directory (aka 0000).
        // So, creating additional soundtrack directories (0001, 0002, etc.)
        // isn't needed.

        // std::string dir = "fffe0000/music/" + get_zero_padded_string(id, 4);

        // if (create_filesystem)
        //     std::filesystem::create_directory(dir);

        // If a folder name wasn't specified, use the soundtrack
        // name instead
        if (!folder.size())
            folder = std::filesystem::path(name).string();

        // Create groups based on the amount of songs in this soundtrack
        std::vector <group_t> groups;

        int count = 0;

        for (const auto& entry : std::filesystem::directory_iterator(folder))
            count++;

        // Create ceil(n/6) groups to allocate for non multiple of 6
        // song counts
        groups.resize(std::ceil((double)count / 6));

        // numSongGroups as described in xboxdevwiki actually
        // contains the amount of tracks
        soundtrack[id].num_songs = count;

        // Fill in general information about the generated
        // groups
        {
            int group_id = 0;

            for (group_t& group : groups) {
                soundtrack[id].song_group_ids[group_id++] = global_group_id;
                group.id = global_group_id++;
                group.soundtrack_id = id;
            }
        }

        // Copy songs to 0000, set metadata
        {
            int id = 0;

            for (const auto& song : std::filesystem::directory_iterator(folder)) {
                // Generate the 8-digit zero-padded
                std::string zpsid = get_zero_padded_string(global_song_id++, 8);

                if (create_filesystem)
                    std::filesystem::copy(song.path(), "fffe0000/music/0000/" + zpsid + ".wma");
                
                // Calculate group and song (inside group) indexes
                int group_index = std::ceil((double)(id + 1) / 6) - 1,
                    song_index = id % 6;

                group_t& group = groups[group_index];

                // Set song ID
                group.song_id[song_index] = std::stoi(zpsid, nullptr, 16);

                // Set track name
                std::wstring song_name = L"Track " + std::to_wstring(id + 1);

                std::memcpy(&group.song_name[song_index * 32], song_name.c_str(), song_name.size() * sizeof(wchar_t));

                // Increment song ID
                id++;
            }
        }

        // Push generated groups
        for (group_t& group : groups)
            global_groups.push_back(group);
    }

    size_t write() {
        std::ofstream f("fffe0000/music/ST.DB", std::ios::binary);

        f.write((char*)&header, sizeof(header_t));

        for (int i = 0; i < 100; i++)
            f.write((char*)&soundtrack[i], sizeof(soundtrack_t));

        f.write((char*)global_groups.data(), global_groups.size() * sizeof(group_t));

        return f.tellp();
    }
}