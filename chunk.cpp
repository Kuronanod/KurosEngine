#include "world.h"

void intializechunk(Chunk& chunk,int chunk_position_x,int chunk_position_y){
    chunk.position_x = chunk_position_x;
    chunk.position_y = chunk_position_y;

    for (int y = 0; y < chunk_size; y++){
        for (int x = 0; x < chunk_size; x++){

            tile& t = chunk.tiles[y][x];
            t.id = (y % 2 == 0) ? 0 : 1;
            t.solid = (t.id == 1);
            t.position_x = (chunk_position_x * chunk_size * tile_size) + (x * tile_size);
            t.position_y = (chunk_position_y * chunk_size * tile_size) + (y * tile_size);

        }
    }

}
