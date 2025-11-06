#include "render.h"

void draw_tile(int tile_position_x,int tile_position_y,int tile_id){

    const int tile_size = 50;

    if (tile_id == 0 ) glColor3f(0.2f,0.8f,0.2f); //grass(green)
    else if (tile_id == 1) glColor3f(0.6f,0.6f,0.6f); //dirt(brown)
    else if (tile_id == 2) glColor3f(0.2f,0.2f,0.8f); //water(blue)
    else if (tile_id == 3) glColor3f(1.0f,1.0f,0.0f); //sand(yellow)
    else glColor3f(1.0f,0.0f,0.0f);

    glBegin(GL_QUADS);
        glVertex2f(tile_position_x , tile_position_y);
        glVertex2f(tile_position_x + tile_size , tile_position_y);
        glVertex2f(tile_position_x + tile_size , tile_position_y + tile_size);
        glVertex2f(tile_position_x , tile_position_y + tile_size);
    glEnd();

    //drawgrid between tiles
    glColor3f(0.0f,0.0f,0.0f);
    glLineWidth(1.0f);

    glBegin(GL_LINE_LOOP);
        glVertex2f(tile_position_x , tile_position_y);
        glVertex2f(tile_position_x + tile_size , tile_position_y);
        glVertex2f(tile_position_x + tile_size , tile_position_y + tile_size);
        glVertex2f(tile_position_x , tile_position_y + tile_size);
    glEnd();

}

void draw_chunk_border(float chunk_position_x,float chunk_position_y,int chunk_width,int chunk_height,int tile_size){

    float width = chunk_width * tile_size;
    float height = chunk_height * tile_size;

    glColor3f(1.0f,1.0f,0.0f);
    glLineWidth(3.0f);

    glBegin(GL_LINE_LOOP);
        glVertex2f(chunk_position_x , chunk_position_y);
        glVertex2f(chunk_position_x + width , chunk_position_y);
        glVertex2f(chunk_position_x + width , chunk_position_y + height);
        glVertex2f(chunk_position_x , chunk_position_y + height);
    glEnd();

}