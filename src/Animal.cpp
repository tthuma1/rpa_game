#include <Animal.h>
#include <Config.h>
#include <iostream>

void Animal::init(SDL_Renderer *renderer)
{
    this->renderer = renderer;

    rect = {0, 0, ANIMAL_WIDTH, ANIMAL_HEIGHT};

    char tmp[40];
    for (int i = 0; i < 2; i++)
    {
        strcpy(tmp, "assets/animal/");
        strcat(tmp, paths[i]);

        // std::cout << tmp << std::endl;

        SDL_Surface *surface = IMG_Load(tmp);
        assets[i] = SDL_CreateTextureFromSurface(this->renderer, surface);
    }
}

void Animal::draw()
{
    curr_time = std::chrono::steady_clock::now();

    if ((curr_time - last_frame).count() > 200000000)
    {
        frame = (frame + 1) % 2;

        last_frame = curr_time;
    }

    // std::cout << direction * 4 + frame << std::endl;

    SDL_RenderCopy(this->renderer, assets[frame], NULL, &rect);
}

void Animal::set_direction(int a)
{
    direction = a;
}

void Animal::move()
{
    if (direction == 1)
        rect.x -= SPEED;
    else if (direction == 2)
        rect.x += SPEED;
}

void Animal::move_up(int a)
{
    rect.y -= a;
}

void Animal::move_down(int a)
{
    rect.y += a;
}

bool Animal::detect_player_collision(SDL_Rect player_rect)
{
    SDL_bool collision = SDL_HasIntersection(&rect, &player_rect);

    if (collision)
    {
        return true;
    }

    return false;
}

void Animal::reset(int x, int y)
{
    rect.x = x;
    rect.y = y;
}

SDL_Rect Animal::get_rect()
{
    return rect;
}