#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

#include <Background.h>
#include <Config.h>

Background::Background()
{
}

void Background::init(SDL_Renderer *renderer)
{
    std::cout << "Background initialized\n";
    rect = {0, -BG_HEIGHT + HEIGHT, BG_WIDTH, BG_HEIGHT};
    this->renderer = renderer;

    SDL_Surface *surface = IMG_Load("assets/background.png");
    background = SDL_CreateTextureFromSurface(this->renderer, surface);

    SDL_RenderCopy(this->renderer, background, NULL, &rect);
}

void Background::reset(int x)
{
    rect.x = x;
    rect.y = -BG_HEIGHT + HEIGHT;
}

void Background::set_direction(int a)
{
    direction = a;
}

void Background::move()
{
    if (direction == 1)
        rect.x -= SPEED;
    else if (direction == 2)
        rect.x += SPEED;
    detect_border_collision();
}
void Background::move_up(int a)
{
    rect.y -= a;
}

void Background::move_down(int a)
{
    rect.y += a;
}

void Background::draw()
{
    SDL_RenderCopy(this->renderer, background, NULL, &rect);
}

void Background::detect_border_collision()
{
    if (rect.x > 0)
        rect.x = 0;
    else if (rect.x < -BG_WIDTH + WIDTH)
        rect.x = -BG_WIDTH + WIDTH;

    if (rect.y < -BG_HEIGHT + HEIGHT)
        rect.y = -BG_HEIGHT + HEIGHT;
}