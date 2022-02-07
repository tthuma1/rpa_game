#include <Health.h>
#include <iostream>
#include <Config.h>

void Health::init(SDL_Renderer *renderer)
{
    // std::cout << "Health initialized\n";
    rect = {10, 10, HEART_WIDTH, HEART_WIDTH};
    this->renderer = renderer;

    SDL_Surface *surface = IMG_Load("assets/heart.png");
    asset = SDL_CreateTextureFromSurface(this->renderer, surface);

    SDL_RenderCopy(this->renderer, asset, NULL, &rect);
}

void Health::draw()
{
    for (int i = 0; i < health; i++)
    {
        SDL_RenderCopy(this->renderer, asset, NULL, &rect);
        rect.x += HEART_WIDTH + 7;
    }

    rect.x = 10;
}

void Health::decrease()
{
    curr_time = std::chrono::steady_clock::now();

    if ((curr_time - last_hit).count() > 1000000000) // > 1000 ms, 1 s
    {
        health--;
        last_hit = curr_time;
    }
}

void Health::increase()
{
    health++;
}

void Health::reset()
{
    health = MAX_HEALTH;
}

int Health::get_health()
{
    return health;
}