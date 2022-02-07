#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <Ground.h>
#include <Config.h>

void Ground::init(SDL_Renderer *renderer)
{
    rect = {0, GROUND_Y, BG_WIDTH, 350};
    this->renderer = renderer;

    SDL_Surface *surface = IMG_Load("assets/ground.png");
    ground = SDL_CreateTextureFromSurface(this->renderer, surface);

    SDL_RenderCopy(this->renderer, ground, NULL, &rect);
}

void Ground::reset(int x)
{
    rect.x = x;
    rect.y = GROUND_Y;
}

void Ground::set_direction(int a)
{
    direction = a;
}

bool Ground::move()
{
    if (direction == 1)
        rect.x -= SPEED;
    else if (direction == 2)
        rect.x += SPEED;
    return detect_border_collision();
}

void Ground::move_up(int a)
{
    rect.y -= a;
}

void Ground::move_down(int a)
{
    rect.y += a;
}

void Ground::draw()
{
    SDL_RenderCopy(this->renderer, ground, NULL, &rect);
}

bool Ground::detect_border_collision()
{
    if (rect.x > 0)
        rect.x = 0;
    else if (rect.x < -BG_WIDTH + 1000)
        rect.x = -BG_WIDTH + 1000;
    else
        return true; // did move

    return false; // did not move
}

bool Ground::detect_player_collision(SDL_Rect player_rect)
{
    SDL_bool collision = SDL_HasIntersection(&rect, &player_rect);

    if (collision)
    {
        // rect.y = player_rect.y + PL_HEIGHT;
        return true;
    }

    return false;
}