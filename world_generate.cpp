#include "world.h"
#include <cstdlib>
#include "world_generate.h"

Chunk generate_chunk(int chunk_position_x,int chunk_position_y){
    Chunk map_chunk;

    map_chunk.position_x = chunk_position_x * chunk_size * tile_size;
    map_chunk.position_y = chunk_position_y * chunk_size * tile_size;

    for (int y = 0 ; y < chunk_size; y++){
        for (int x = 0 ; x < chunk_size ; x++){

            int random_id = rand() % 4;

            map_chunk.tiles[y][x].id = random_id;
            map_chunk.tiles[y][x].solid = false;
            map_chunk.tiles[y][x].position_x = chunk_position_x * chunk_size * tile_size + x * tile_size;
            map_chunk.tiles[y][x].position_y = chunk_position_y * chunk_size * tile_size + y * tile_size;
        }
    }
    return map_chunk;
}