#pragma once

#include <SDL2/SDL.h>

#include <Platform.h>

class Background
{
private:
    SDL_Renderer *renderer;
    SDL_Texture *background;

    SDL_Rect rect;

    int direction = 0; // 0 = stop, 1 = right, 2 = left

public:
    Background();
    void init(SDL_Renderer *renderer);

    void draw();
    void set_direction(int);
    void move();
    void move_up(int);
    void move_down(int);
    void detect_border_collision();
    SDL_Rect get_rect();

    void reset(int x);
    void reset(int x, int y);
};