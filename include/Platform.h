#pragma once

#include <SDL2/SDL.h>
#include <ctime>

class Platform
{
private:
    SDL_Renderer *renderer;
    SDL_Texture *asset;

    SDL_Rect rect;

    int direction = 0; // 0 = stop, 1 = right, 2 = left

public:
    void init(SDL_Renderer *renderer);
    void reset(int x, int y);

    void draw();
    void set_direction(int);
    void move();
    void move_up(int);
    void move_down(int);
    bool detect_player_collision(SDL_Rect);
    SDL_Rect get_rect();
};
