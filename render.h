#ifndef RENDER_H
#define RENDER_H

#include "GL/gl.h"

void draw_tile(int tile_position_x,int tile_position_y,int tile_id);
void draw_chunk_border(float chunk_position_x,float chunk_position_y,int chunk_width,int chunk_height,int tile_size);

#endif