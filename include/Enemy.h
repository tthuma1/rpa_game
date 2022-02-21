#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <Config.h>
#include <chrono>

class Enemy
{
private:
    SDL_Renderer *renderer;
    SDL_Texture *assets[8];

    SDL_Rect rect;

    short direction; // 0 = right, 1 = left
    short frame = 0;
    int relative_x, move_count = 0;
    int platform_num; // na kateri platformi je enemy

    std::chrono::steady_clock::time_point last_frame = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point curr_time;

    char paths[8][20] = {
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
    void init(SDL_Renderer *renderer, int);

    void draw(int, int);
    SDL_Rect get_rect();
    void move();
    int get_platform_num();

    void detect_end_collision();
    bool detect_player_collision(SDL_Rect);

    void reset();
};