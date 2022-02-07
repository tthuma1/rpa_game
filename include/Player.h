#pragma once

#include <SDL2/SDL.h>
#include <Config.h>
#include <chrono>

class Player
{
private:
    SDL_Renderer *renderer;
    SDL_Texture *assets[12];

    SDL_Rect rect;

    int vel_y = -10;
    short direction = 0; // 0 = stop, 1 = right, 2 = left
    short frame = 0;
    bool is_on_platform = false;

    std::chrono::steady_clock::time_point last_frame = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point curr_time;

    char paths[12][20] = {
        "idle_0.png",
        "idle_1.png",
        "idle_2.png",
        "idle_3.png",
        "run_0.png",
        "run_1.png",
        "run_2.png",
        "run_3.png",
        "run_left_0.png",
        "run_left_1.png",
        "run_left_2.png",
        "run_left_3.png",
    };

public:
    Player();
    void init(SDL_Renderer *renderer);

    void draw();
    void set_vel_y(int vel);
    int get_vel_y();
    SDL_Rect get_rect();

    void set_direction(int);
    void set_is_on_platform(bool);
    bool get_is_on_platform();
};