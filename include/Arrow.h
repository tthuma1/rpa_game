#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class Arrow
{
private:
    SDL_Renderer *renderer;
    SDL_Texture *assets[4];

    SDL_Rect rect;

    int direction = 0; // 0 = none, 1 = up, 2 = down, 3 = left, 4 = right

    char paths[4][20] = {
        "up.png",
        "down.png",
        "left.png",
        "right.png",
    };

public:
    void init(SDL_Renderer *renderer);

    void draw();
    void set_direction(int);
};