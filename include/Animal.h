#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <chrono>

class Animal
{
private:
    SDL_Renderer *renderer;
    SDL_Texture *assets[2];

    SDL_Rect rect;

    short frame = 0;
    int direction = 0; // 0 = stop, 1 = right, 2 = left

    char paths[2][20] = {
        "idle_0.png",
        "idle_1.png",
    };

    std::chrono::steady_clock::time_point last_frame = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point curr_time;

public:
    void init(SDL_Renderer *renderer);

    void draw();
    void set_direction(int);
    void move();
    void move_up(int);
    void move_down(int);

    void reset(int x, int y);
    SDL_Rect get_rect();

    bool detect_player_collision(SDL_Rect);
};