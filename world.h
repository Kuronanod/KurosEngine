#pragma once
#include <vector>
#include <unordered_map>
#include <mutex>
#include <tuple>
#include <functional>

using namespace std;

const int tile_size = 50;
const int chunk_size = 8;

struct tile{
    int id;
    bool solid;
    float position_x;
    float position_y;
};

struct Chunk {
    tile tiles[chunk_size][chunk_size];
    int position_x;
    int position_y;
};

struct chunk_position{
    int x;
    int y;
    bool operator == (const chunk_position& other) const{
        return x == other.x && y == other.y;
    }
};

struct pair_hash{
    size_t operator() (const chunk_position& p) const {

        size_t h1 = hash<int>()(p.x);
        size_t h2 = hash<int>()(p.y);

        return h1 ^ (h2 + 0x9e3779b97f4a7c15ULL + (h1<<6) + (h1>>2));
    }
};

extern unordered_map<chunk_position , Chunk , pair_hash> world_chunk_map;
extern mutex world_map_mutex;