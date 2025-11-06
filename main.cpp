#include <iostream>
#include <cmath>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include "world.h"
#include "graphic.h"
#include "render.h"
#include "world_generate.h"
#include "chunk_worker.h"

using namespace std;

float player_x = 0.0f;
float player_y = 0.0f;
float camera_x = 0.0f;
float camera_y = 0.0f;
bool show_chunk_border = false;
const int render_distance = 2;
const float movement_speed = 60.0f;

static void get_keypress(GLFWwindow* main_window,int key,int scancode,int action,int mods){

    if (action == GLFW_PRESS or action == GLFW_REPEAT){
        if (key == GLFW_KEY_W){
            player_y -= movement_speed;
        }
        if (key == GLFW_KEY_A){
            player_x -= movement_speed;
        }
        if (key == GLFW_KEY_S){
            player_y += movement_speed;
        }
        if (key == GLFW_KEY_D){
            player_x += movement_speed;
        }
        if (key == GLFW_KEY_F3){
            show_chunk_border = !show_chunk_border;
        }
    }

}

void world_render_distance(){
    int player_chunk_position_x = static_cast<int>(floor(player_x / (chunk_size * tile_size)));
    int player_chunk_position_y = static_cast<int>(floor(player_y / (chunk_size * tile_size)));

    for (int cy = player_chunk_position_y - render_distance ; cy <= player_chunk_position_y + render_distance ; cy++){
        for(int cx = player_chunk_position_x - render_distance ; cx <= player_chunk_position_x + render_distance ; cx++){
            chunk_position key{cx,cy};
            if(world_chunk_map.find(key) == world_chunk_map.end()){
                enqueue_chunk_job(cx,cy);
            }
        }
    }

}

void render_world(){

    float screen_left = camera_x - 500;
    float screen_right = camera_x + 900 + 100;
    float screen_top = camera_y - 500;
    float screen_bottom = camera_y + 600 + 100;

    lock_guard<mutex> lk(world_map_mutex);

    for (auto& pair : world_chunk_map){
        Chunk& chunk = pair.second;

        float chunk_world_position_x = chunk.position_x;
        float chunk_world_position_y = chunk.position_y;
        float chunk_size_px = chunk_size * tile_size;

        if (chunk_world_position_x + chunk_size_px < screen_left ||
            chunk_world_position_x > screen_right ||
            chunk_world_position_y + chunk_size < screen_top ||
            chunk_world_position_y > screen_bottom){

                continue;

            }

        for (int y = 0 ; y < chunk_size ; y++){
            for (int x = 0 ; x < chunk_size ; x++){
                float draw_x = chunk.tiles[y][x].position_x-camera_x;
                float draw_y = chunk.tiles[y][x].position_y-camera_y;

                draw_tile(draw_x,draw_y,chunk.tiles[y][x].id);

            }
        }
        if (show_chunk_border){
            draw_chunk_border(chunk.position_x - camera_x,chunk.position_y - camera_y,chunk_size,chunk_size,tile_size);
        }
        
    }

    cout << " render total chunk load: " << world_chunk_map.size() << endl;

}

void world_unloader(){
    int player_chunk_position_x = static_cast<int>(floor(player_x / (tile_size * chunk_size)));
    int player_chunk_position_y = static_cast<int>(floor(player_y / (tile_size * chunk_size)));

    vector<chunk_position> chunk_to_remove;
    const int unload_distance = render_distance+1;

    for(auto& pair : world_chunk_map){
        const chunk_position& pos = pair.first;
        int dx = abs(pos.x - player_chunk_position_x);
        int dy = abs(pos.y - player_chunk_position_y);

        if(max(dx,dy) > unload_distance){
            chunk_to_remove.push_back(pos);
        }
    }

    if(!chunk_to_remove.empty()){
        for(auto& pos : chunk_to_remove){
            world_chunk_map.erase(pos);
        }
    }
}

int main() {

    //Initialize
    if (!glfwInit()) {
        cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    glfwWindowHint(GLFW_RESIZABLE , GLFW_FALSE);
    GLFWwindow* main_window = glfwCreateWindow(900, 600, "Message From Farland", nullptr, nullptr);

    if (!main_window) {
        cerr << "Failed to create window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(main_window);
    glfwSetKeyCallback(main_window,get_keypress);
    glClearColor(0.5f,0.5f,0.5f,1.0f);
    setup_orthographic_view(900 , 600);

    for(int cy = -1 ; cy <= 1 ; cy++){
        for(int cx = -1 ; cx <= 1 ; cx++){
            world_chunk_map[{cx , cy}] = generate_chunk(cx,cy);
        }
    }
    cout << "world_chunk size: " << world_chunk_map.size() << endl;

    start_chunk_worker(2);
    world_render_distance();
    double previous_time = glfwGetTime();

    // Main loop
    while (!glfwWindowShouldClose(main_window)) { 
        glClear(GL_COLOR_BUFFER_BIT);

        double current_time = glfwGetTime();
        double delta_time = current_time - previous_time;
        previous_time = current_time;

        camera_x = player_x - 450;
        camera_y = player_y - 300;

        world_render_distance();
        world_unloader();
        render_world();

        glfwSwapBuffers(main_window);
        glfwPollEvents();
        
    }

    glfwDestroyWindow(main_window);
    glfwTerminate(); 
    return 0;
}