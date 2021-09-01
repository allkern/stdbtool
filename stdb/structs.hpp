#pragma once

#include <cstdint>

namespace stdb {
#define STDB_SOUNDTRACK_COUNT 100
#define STDB_HEADER_PAD 96
#define STDB_HEADER_MAGIC 0x1
    struct header_t {
        int32_t magic = STDB_HEADER_MAGIC,
                num_soundtracks,
                next_soundtrack_id,
                soundtrack_id[STDB_SOUNDTRACK_COUNT],
                next_song_id;

        char pad[STDB_HEADER_PAD];
    } header;

#define STDB_SOUNDTRACK_MAGIC 0x00021371

// This constant was found by reverse engineering actual files.
// I'd love to know why this struct can save at most exactly 84
// song group IDs.
#define STDB_SOUNDTRACK_SGID_COUNT 84

// Soundtrack names only get 32 viewable characters.
// I'm not sure how many of these 80 characters are actually parsed as
// being part of the name, Probably 32, and the rest is padding.
#define STDB_SOUNDTRACK_NAME_LEN 80

    struct soundtrack_t {
        int32_t magic, // Only fill in magic number when a soundtrack is actually generated
                id,
                num_songs,
                song_group_ids[STDB_SOUNDTRACK_SGID_COUNT],
                total_time_ms;

        // Hack? Not sure how the soundtrack name is stored.
        // I've found that these always (?) start at offset
        // +0x160, a padding of this size will accomplish
        // this.
        wchar_t name[STDB_SOUNDTRACK_NAME_LEN];
    } soundtrack[STDB_SOUNDTRACK_COUNT];

// Each group contains 6 songs, each song gets an ID, length (in ms)
// and a name. Song names may contain at most 32 characters.
#define STDB_GROUP_SONG_COUNT 6
#define STDB_GROUP_SONG_NAME_LEN 32

#define STDB_GROUP_MAGIC 0x00031073

// ST.DB files ripped from Original Xboxes initialize the
// single int32 padding in groups to 1 (no idea why).
// This is probably not needed, I'll keep it just for accuracy sake.
#define STDB_GROUP_PAD_MAGIC 0x1

    struct group_t {
        int32_t magic = STDB_GROUP_MAGIC,
                soundtrack_id,
                id,
                padding = STDB_GROUP_PAD_MAGIC,
                song_id[STDB_GROUP_SONG_COUNT],
                song_time_ms[STDB_GROUP_SONG_COUNT];
        wchar_t song_name[STDB_GROUP_SONG_NAME_LEN * STDB_GROUP_SONG_COUNT];
        
        char pad[64];
    };
}