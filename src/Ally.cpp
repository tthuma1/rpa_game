#include <Ally.h>
#include <Config.h>
#include <iostream>

void Ally::init(SDL_Renderer *renderer)
{
    this->renderer = renderer;

    rect = {0, 0, PL_WIDTH, PL_HEIGHT};

    char tmp[40];
    for (int i = 0; i < 3; i++)
    {
        strcpy(tmp, "assets/ally/");
        strcat(tmp, paths[i]);

        // std::cout << tmp << std::endl;

        SDL_Surface *surface = IMG_Load(tmp);
        assets[i] = SDL_CreateTextureFromSurface(this->renderer, surface);
    }
}

void Ally::draw()
{
    curr_time = std::chrono::steady_clock::now();

    if ((curr_time - last_frame).count() > 130000000) // 130 ms
    {
        frame = (frame + 1) % 3;

        last_frame = curr_time;
    }

    // std::cout << direction * 4 + frame << std::endl;

    SDL_RenderCopy(this->renderer, assets[frame], NULL, &rect);
}

void Ally::set_direction(int a)
{
    direction = a;
}

void Ally::move()
{
    if (direction == 1)
        rect.x -= SPEED;
    else if (direction == 2)
        rect.x += SPEED;
}

void Ally::move_up(int a)
{
    rect.y -= a;
}

void Ally::move_down(int a)
{
    rect.y += a;
}

bool Ally::detect_player_collision(SDL_Rect player_rect)
{
    SDL_bool collision = SDL_HasIntersection(&rect, &player_rect);

    if (collision)
    {
        return true;
    }

    return false;
}

void Ally::reset(int x, int y)
{
    rect.x = x;
    rect.y = y;
}

SDL_Rect Ally::get_rect()
{
    return rect;
}