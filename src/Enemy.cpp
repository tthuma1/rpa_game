#include <Enemy.h>
#include <iostream>

void Enemy::init(SDL_Renderer *renderer, int plat_num)
{
    std::cout << "Enemy initilized\n";
    this->renderer = renderer;
    platform_num = plat_num;

    relative_x = rand() % PLAT_WIDTH; // [min_x, max_x)
    direction = rand() % 2;

    rect = {0, 0, PL_WIDTH, PL_HEIGHT};

    char tmp[40];
    for (int i = 0; i < 8; i++)
    {
        strcpy(tmp, "assets/enemy/");
        strcat(tmp, paths[i]);

        // std::cout << tmp << std::endl;

        SDL_Surface *surface = IMG_Load(tmp);
        assets[i] = SDL_CreateTextureFromSurface(this->renderer, surface);
    }
}

void Enemy::draw(int plat_x, int plat_y)
{
    curr_time = std::chrono::steady_clock::now();

    if ((curr_time - last_frame).count() > 100000000) // > 100 ms, 0.1 s
    {
        frame = (frame + 1) % 4;

        last_frame = curr_time;
    }

    rect.x = plat_x + relative_x;
    rect.y = plat_y - PL_HEIGHT + 3;

    // std::cout << direction * 4 + frame << std::endl;

    SDL_RenderCopy(this->renderer, assets[direction * 4 + frame], NULL, &rect);
}

void Enemy::move()
{
    if (move_count % 2) // make him go half as fast, rect.x can't be float
    {
        // std::cout << move_count << std::endl;
        if (direction == 0)
            relative_x += ENEMY_SPEED;
        else if (direction == 1)
            relative_x -= ENEMY_SPEED;
        detect_end_collision();
    }

    move_count++;
}

void Enemy::detect_end_collision()
{
    // change direction on platform end
    if (relative_x <= 0)
        direction = 0;
    else if (relative_x >= PLAT_WIDTH - PL_WIDTH)
        direction = 1;
}

SDL_Rect Enemy::get_rect()
{
    return rect;
}

bool Enemy::detect_player_collision(SDL_Rect player_rect)
{
    SDL_bool collision = SDL_HasIntersection(&rect, &player_rect);

    if (collision)
    {
        return true;
    }

    return false;
}

int Enemy::get_platform_num()
{
    return platform_num;
}

void Enemy::reset()
{
    relative_x = 0;
}