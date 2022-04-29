#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <chrono>

struct AllyStats
{
    int x, y;
};

class Ally
{
private:
    SDL_Renderer *renderer;
    SDL_Texture *assets[3];

    SDL_Rect rect;

    short frame = 0;
    int direction = 0; // 0 = stop, 1 = right, 2 = left

    char paths[3][20] = {
        "idle_0.png",
        "idle_1.png",
        "idle_2.png",
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