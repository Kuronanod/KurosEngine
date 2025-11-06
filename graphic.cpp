#include "GL/gl.h"
#include "graphic.h"

void setup_orthographic_view(int screen_width,int screen_height){
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(0,screen_width,screen_height,0,-1,1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glViewport(0,0,screen_width,screen_height);
}
