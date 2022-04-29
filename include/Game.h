#pragma once

#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include <ctime>
#include <cstring>
#include <cstdio>
#include <fstream>

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
#include <Menu.h>
#include <Tutorial.h>

struct GameStats
{
    int level, play_time, animals_left, health,
        ground_x, ground_y, background_y,
        animal_x, animal_y, ally_x, ally_y,
        platforms_x[13], platforms_y[13],
        num_enemies, enemies_platforms[13], enemies_directions[13],
        enemies_x[13];
    struct PlayerStats player_stats;
};

class Game
{
private:
    SDL_Window *window;
    SDL_Renderer *renderer;

    bool is_running;
    bool first_finish = true;
    int level = 0;                         // 0 do 3
    int num_platforms[4] = {5, 8, 11, 13}; // glede na level
    int num_animals[4] = {3, 5, 7, 9};
    int animals_left;
    int screen; // 0 = game, 1 = game over, 2 = menu, 3 = tutorial
    bool just_died;
    int add_play_time;
    PlayerStats player_stats;

    Player player;
    Background background;
    Ground ground;
    std::vector<Platform> platforms;
    std::vector<Enemy> enemies;
    Health health;
    Animal animal;
    Ally ally;
    Arrow arrow;
    Text preostale, level_curr, level_done, time_text;
    GameOver game_over_screen;
    Menu menu_screen;
    Tutorial tutorial_screen;

    std::chrono::steady_clock::time_point last_frame = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point curr_time, finish_time, start_game_time, curr_game_time, last_move_time;

public:
    Game();
    void run();
    void replay();
    void game_screen();
    void game_over();
    void menu();
    void tutorial();
    void init_level();
    void write_result();

    void save_game();
    void load_game();
};