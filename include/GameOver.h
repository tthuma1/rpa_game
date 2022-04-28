#pragma once

#include <fstream>
#include <iostream>

#include <SDL2/SDL.h>
#include <Text.h>
#include <PlayerStats.h>

class GameOver
{
private:
    SDL_Renderer *renderer;

    Text text1, text2, text3;
    Text results[5];

public:
    void init(SDL_Renderer *renderer);
    void draw();
    void won();
    void reset();
    void load_data();
};