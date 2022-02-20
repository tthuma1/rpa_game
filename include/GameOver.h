#pragma once

#include <SDL2/SDL.h>

#include <Text.h>

class GameOver
{
private:
    SDL_Renderer *renderer;

    Text text1, text2, text3;

public:
    void init(SDL_Renderer *renderer);
    void draw();
};