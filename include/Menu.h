#pragma once

#include <SDL2/SDL.h>
#include <Background.h>
#include <Text.h>

class Menu
{
private:
    SDL_Renderer *renderer;

    SDL_Texture *background, *arrow;
    SDL_Rect rect_bg, rect_arrow;

    Text title, options[3];
    int position;

public:
    void init(SDL_Renderer *renderer);
    void draw();

    void move_down();
    void move_up();
    int get_position();
    void change_title(char text[]);
};