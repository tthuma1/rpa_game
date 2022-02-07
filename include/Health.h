#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <Config.h>
#include <chrono>

class Health
{
private:
    SDL_Renderer *renderer;
    SDL_Texture *asset;

    SDL_Rect rect;
    int health = MAX_HEALTH;

    std::chrono::steady_clock::time_point last_hit = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point curr_time;

public:
    void init(SDL_Renderer *renderer);

    void draw();
    void decrease();
    void increase();
    void reset();
    int get_health();
};