#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <cstring>

#include <Player.h>

Player::Player()
{
    rect = {PL_X, PL_Y, PL_WIDTH, PL_HEIGHT};
}

void Player::init(SDL_Renderer *renderer)
{
    std::cout << "Player initialized\n";
    this->renderer = renderer;

    char tmp[40];
    for (int i = 0; i < 12; i++)
    {
        strcpy(tmp, "assets/player/");
        strcat(tmp, paths[i]);

        // std::cout << tmp << std::endl;

        SDL_Surface *surface = IMG_Load(tmp);
        assets[i] = SDL_CreateTextureFromSurface(this->renderer, surface);
    }
}

void Player::draw()
{
    curr_time = std::chrono::steady_clock::now();

    if ((curr_time - last_frame).count() > 100000000) // > 100 ms, 0.1 s
    {
        frame = (frame + 1) % 4;

        last_frame = curr_time;
    }

    // std::cout << direction * 4 + frame << std::endl;

    SDL_RenderCopy(this->renderer, assets[direction * 4 + frame], NULL, &rect);
}

void Player::set_vel_y(int vel)
{
    vel_y = vel;
}

int Player::get_vel_y()
{
    return vel_y;
}

SDL_Rect Player::get_rect()
{
    return rect;
}

void Player::set_direction(int x)
{
    direction = x;
}

void Player::set_is_on_platform(bool is)
{
    is_on_platform = is;
}

bool Player::get_is_on_platform()
{
    return is_on_platform;
}

void Player::reset(int x, int y)
{
    rect.x = x;
    rect.y = y;
}