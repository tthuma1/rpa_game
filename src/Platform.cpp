#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

#include <Platform.h>
#include <Config.h>

void Platform::init(SDL_Renderer *renderer)
{
    this->renderer = renderer;

    SDL_Surface *surface = IMG_Load("assets/platform.png");
    asset = SDL_CreateTextureFromSurface(this->renderer, surface);

    rect = {0, 0, PLAT_WIDTH, PLAT_HEIGHT};

    SDL_RenderCopy(this->renderer, asset, NULL, &rect);
}

void Platform::reset(int x, int y)
{
    rect.x = x;
    rect.y = y;
}

void Platform::set_direction(int a)
{
    direction = a;
}

void Platform::move()
{
    if (direction == 1)
        rect.x -= SPEED;
    else if (direction == 2)
        rect.x += SPEED;
}

void Platform::move_up(int a)
{
    rect.y -= a;
}

void Platform::move_down(int a)
{
    rect.y += a;
}

void Platform::draw()
{
    SDL_RenderCopy(this->renderer, asset, NULL, &rect);
}

bool Platform::detect_player_collision(SDL_Rect player_rect)
{
    SDL_bool collision = SDL_HasIntersection(&rect, &player_rect);

    // 3/4 of player's body is aboe platform
    if (collision && (player_rect.y + 3 * PL_HEIGHT / 4) < rect.y)
    {
        return true;
    }

    return false;
}

SDL_Rect Platform::get_rect()
{
    return rect;
}