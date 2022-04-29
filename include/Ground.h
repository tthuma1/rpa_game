#pragma once

#include <SDL2/SDL.h>

class Ground
{
private:
    SDL_Renderer *renderer;
    SDL_Texture *ground;

    SDL_Rect rect;

    int direction = 0; // 0 = stop, 1 = right, 2 = left

public:
    void init(SDL_Renderer *renderer);

    void draw();
    void set_direction(int);
    bool move();
    void move_up(int);
    void move_down(int);
    bool detect_border_collision();
    bool detect_player_collision(SDL_Rect player_rect);

    void reset(int x);
    void reset(int x, int y);
    SDL_Rect get_rect();
};