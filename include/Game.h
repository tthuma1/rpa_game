#pragma once

#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include <ctime>

#include <Player.h>
#include <Background.h>
#include <Ground.h>
#include <Platform.h>
#include <Enemy.h>
#include <Health.h>
#include <Animal.h>
#include <Text.h>
#include <Ally.h>
#include <Arrow.h>
#include <GameOver.h>

class Game
{
private:
    SDL_Window *window;
    SDL_Renderer *renderer;

    bool is_running;
    bool first_finish = true;
    int frame_count = 0;
    int level = 0;                         // 0 do 3
    int num_platforms[4] = {5, 8, 11, 13}; // glede na level
    int num_animals[4] = {3, 5, 7, 9};
    int animals_left;
    int screen = 0; // 0 = game, 1 = game over

    Player player;
    Background background;
    Ground ground;
    std::vector<Platform> platforms;
    std::vector<Enemy> enemies;
    Health health;
    Animal animal;
    Ally ally;
    Arrow arrow;
    Text preostale;
    Text level_done;
    GameOver game_over_screen;

    std::chrono::steady_clock::time_point curr_time;
    std::chrono::steady_clock::time_point finish_time;

public:
    Game();
    void run();
    void game_screen();
    void game_over();
    void init_level();
};