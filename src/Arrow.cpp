#include <Arrow.h>
#include <Config.h>
#include <iostream>

void Arrow::init(SDL_Renderer *renderer)
{
    this->renderer = renderer;

    rect = {-100, 0, 75, 75};

    char tmp[40];
    for (int i = 0; i < 4; i++)
    {
        strcpy(tmp, "assets/arrow/");
        strcat(tmp, paths[i]);

        // std::cout << tmp << std::endl;

        SDL_Surface *surface = IMG_Load(tmp);
        assets[i] = SDL_CreateTextureFromSurface(this->renderer, surface);
    }
}

void Arrow::draw()
{
    if (direction == 0)
        return;

    if (direction == 1)
    {
        rect.x = WIDTH / 2 - 65;
        rect.y = 0;
    }
    else if (direction == 2)
    {
        rect.x = WIDTH / 2 - 65;
        rect.y = HEIGHT - 75;
    }
    else if (direction == 3)
    {
        rect.x = 0;
        rect.y = HEIGHT / 2 - 50;
    }
    else if (direction == 4)
    {
        rect.x = WIDTH - 75;
        rect.y = HEIGHT / 2 - 50;
    }

    SDL_RenderCopy(this->renderer, assets[direction - 1], NULL, &rect);
}

void Arrow::set_direction(int a)
{
    direction = a;
}