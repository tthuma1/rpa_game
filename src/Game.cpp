#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <Game.h>
#include <Config.h>

Game::Game()
{
    srand(time(NULL));
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        std::cout << SDL_GetError() << '\n';
        throw "SDL INIT ERROR";
    }
    TTF_Init();

    window = SDL_CreateWindow("Osvoboditelj", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, 0);

    // std::cout << "Game init\n";

    is_running = true;
    screen = 2;

    player.init(renderer);
    health.init(renderer);
    arrow.init(renderer);
    background.init(renderer);
    ground.init(renderer);
    for (int i = 0; i < 13; i++)
    {
        Platform plat;
        platforms.push_back(plat);
        platforms[i].init(renderer);
    }
    animal.init(renderer);
    ally.init(renderer);

    char tmp_text[] = "Stopnja je opravljena!";
    level_done.init(renderer, 300, HEIGHT / 2 - 100, tmp_text);

    game_over_screen.init(renderer);
    menu_screen.init(renderer);
    tutorial_screen.init(renderer);
}

void Game::init_level()
{
    // std::cout << "level init begin\n";
    std::ofstream datao_replay("replay.bin", std::ios::binary);
    // start_x je med enim screenom od levega in desnega roba
    int start_x = rand() % (BG_WIDTH - 3 * WIDTH) - BG_WIDTH + 2 * WIDTH; // [-BG_WIDTH + 2 * WIDTH, -WIDTH) = [-3000, -1000]
    datao_replay.write((char *)&start_x, sizeof(start_x));
    // std::cout << start_x << std::endl;

    // level = 3;

    // std::cout << "Start init\n";

    animals_left = num_animals[level];
    just_died = false;

    // create text for preostale
    char tmp_text[40] = "Preostale zivali:  ";
    tmp_text[strlen(tmp_text) - 1] = '0' + animals_left;
    preostale.init(renderer, 20, 10, tmp_text);

    // create text for level_curr
    strcpy(tmp_text, "Stopnja   od 4");
    tmp_text[8] = '1' + level;
    // std::cout << tmp_text << std::endl;
    level_curr.init(renderer, 20, 50, tmp_text);

    strcpy(tmp_text, "0 s");
    time_text.init(renderer, 20, 90, tmp_text);

    background.reset(start_x);
    ground.reset(start_x);

    enemies.clear();
    for (int i = 0; i < num_platforms[level]; i++)
    {

        bool is_on_plat = false;
        int tmp = rand() % 100;
        if (tmp < 80) // 80 % chance of enemy on platform
        {
            is_on_plat = true;
            datao_replay.write((char *)&is_on_plat, sizeof(is_on_plat));
            datao_replay.close();

            Enemy enemy;
            enemy.init(renderer, i);
            enemies.push_back(enemy);

            datao_replay.open("replay.bin", std::ios::binary | std::ios::app);
        }
        else
            datao_replay.write((char *)&is_on_plat, sizeof(is_on_plat));
    }

    // std::cout << "Mid init\n";
    // platforms.clear();

    // prva platforma je max polovico zaslona levo ali cetrtino zaslona desno od igralca
    int plat_x;
    do
    {
        plat_x = PL_X + rand() % (3 * WIDTH / 4) - WIDTH / 2;            // [-WIDTH/2, WIDTH/4)
    } while (plat_x >= PL_X - PLAT_WIDTH && start_x <= PL_X + PL_WIDTH); // da se platforma na spawna tam kjer je igralec
    int plat_y = GROUND_Y - 100 - rand() % 50;

    for (int i = 0; i < num_platforms[level]; i++)
    {
        // std::cout << plat_x << " " << plat_y << std::endl;
        platforms[i].reset(plat_x, plat_y);

        datao_replay.write((char *)&plat_x, sizeof(plat_x));
        datao_replay.write((char *)&plat_y, sizeof(plat_y));

        short new_direction = rand() % 10; // right = 0-4, left = 4-9
        // 50-50 chance for each direction
        if (plat_x < 1 * WIDTH + start_x)
            new_direction = 0; // go right, so platforms can't spawn out of bounds
        else if (plat_x - start_x > BG_WIDTH - 1.5 * WIDTH)
            new_direction = 5; // go left, so platforms can't spawn out of bounds

        if (new_direction >= 0 && new_direction <= 4)
            plat_x += PLAT_WIDTH + 50 + rand() % 60; // right
        else
            plat_x -= rand() % (PLAT_WIDTH + 110); // left

        plat_y -= rand() % 60 + 100;
    }

    // first animal position
    int tmp = rand() % num_platforms[level];
    // std::cout << "plat: " << tmp << std::endl;

    SDL_Rect tmp_rect = platforms[tmp].get_rect();
    int tmp_relative_x = rand() % (PLAT_WIDTH - ANIMAL_WIDTH);

    struct AnimalStats tmp_animal;
    tmp_animal.x = tmp_rect.x + tmp_relative_x;
    tmp_animal.y = tmp_rect.y - ANIMAL_HEIGHT + 3;
    datao_replay.write((char *)&tmp_animal, sizeof(tmp_animal));

    animal.reset(tmp_animal.x, tmp_animal.y);

    // first ally position
    tmp = rand() % num_platforms[level];
    tmp_rect = platforms[tmp].get_rect();
    tmp_relative_x = rand() % (PLAT_WIDTH - PL_WIDTH);

    struct AllyStats tmp_ally;
    tmp_ally.x = tmp_rect.x + tmp_relative_x;
    tmp_ally.y = tmp_rect.y - PL_HEIGHT + 3;
    datao_replay.write((char *)&tmp_ally, sizeof(tmp_ally));

    ally.reset(tmp_ally.x, tmp_ally.y);

    // resetting values
    ground.set_direction(0);
    background.set_direction(0);
    player.set_direction(0);
    for (int i = 0; i < num_platforms[level]; i++)
        platforms[i].set_direction(0);
    animal.set_direction(0);
    ally.set_direction(0);

    health.reset();
    player.set_vel_y(-10);

    if (level == 0)
    {
        start_game_time = std::chrono::steady_clock::now();
        player_stats.points = 0;
        add_play_time = 0;
        // clean saved file
        std::ofstream datao_save("saved.bin", std::ios::binary);
        datao_save.close();
    }

    last_move_time = std::chrono::steady_clock::now();
    datao_replay.close();
    // std::cout << "End init\n";
}

void Game::run()
{
    while (is_running)
    {
        switch (screen)
        {
        case 0:
            this->game_screen();
            break;
        case 1:
            this->game_over();
            break;
        case 2:
            this->menu();
            break;
        case 3:
            this->tutorial();
            break;
        }

        // SDL_RenderPresent(renderer);
    }
}

void Game::game_screen()
{
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0)
    {
        // std::cout << e.type << std::endl;
        if (e.type == SDL_QUIT)
            is_running = false;
        else if (e.type == SDL_KEYDOWN)
        {
            std::ofstream datao_replay("replay.bin", std::ios::binary | std::ios::app);
            int time_span_move = (curr_time - last_move_time).count();
            int direction;

            switch (e.key.keysym.sym)
            {
            case SDLK_d:
            case SDLK_RIGHT:
                curr_time = std::chrono::steady_clock::now();
                time_span_move = (curr_time - last_move_time).count();
                last_move_time = std::chrono::steady_clock::now();
                direction = 1;
                datao_replay.write((char *)&time_span_move, sizeof(time_span_move));
                datao_replay.write((char *)&direction, sizeof(int));

                ground.set_direction(1);
                background.set_direction(1);
                player.set_direction(1);
                for (int i = 0; i < num_platforms[level]; i++)
                    platforms[i].set_direction(1);
                animal.set_direction(1);
                ally.set_direction(1);

                break;
            case SDLK_a:
            case SDLK_LEFT:
                curr_time = std::chrono::steady_clock::now();
                time_span_move = (curr_time - last_move_time).count();
                last_move_time = std::chrono::steady_clock::now();
                direction = 2;
                datao_replay.write((char *)&time_span_move, sizeof(time_span_move));
                datao_replay.write((char *)&direction, sizeof(int));

                ground.set_direction(2);
                background.set_direction(2);
                player.set_direction(2);
                for (int i = 0; i < num_platforms[level]; i++)
                    platforms[i].set_direction(2);
                animal.set_direction(2);
                ally.set_direction(2);

                break;
            case SDLK_j:
            case SDLK_s:
            case SDLK_DOWN:
                curr_time = std::chrono::steady_clock::now();
                time_span_move = (curr_time - last_move_time).count();
                last_move_time = std::chrono::steady_clock::now();
                direction = 0;
                datao_replay.write((char *)&time_span_move, sizeof(time_span_move));
                datao_replay.write((char *)&direction, sizeof(int));

                ground.set_direction(0);
                background.set_direction(0);
                player.set_direction(0);
                for (int i = 0; i < num_platforms[level]; i++)
                    platforms[i].set_direction(0);
                animal.set_direction(0);
                ally.set_direction(0);

                break;
            case SDLK_w:
            case SDLK_SPACE:
            case SDLK_UP:
                curr_time = std::chrono::steady_clock::now();
                time_span_move = (curr_time - last_move_time).count();
                last_move_time = std::chrono::steady_clock::now();
                direction = 3;
                datao_replay.write((char *)&time_span_move, sizeof(time_span_move));
                datao_replay.write((char *)&direction, sizeof(int));

                // std::cout << "space " << player.get_vel_y() << std::endl;
                if (player.get_vel_y() == 0) // no double jumping
                    player.set_vel_y(20);

                break;
            case SDLK_ESCAPE:
                // std::cout << "escape\n";
                this->save_game();
                screen = 2;
                break;
            }

            // std::cout << (time_span_move < 0) << std::endl;
            datao_replay.close();
        }
    }
    // datao_replay.close();

    curr_time = std::chrono::steady_clock::now();

    if ((curr_time - last_frame).count() > 10000000) // > 10 ms, 100 Hz
    {
        SDL_RenderClear(renderer);

        int player_vel_y = player.get_vel_y();

        if (player_vel_y > 0)
        {
            ground.move_down(player_vel_y);
            background.move_down(player_vel_y);
            animal.move_down(player_vel_y);
            ally.move_down(player_vel_y);

            for (int i = 0; i < num_platforms[level]; i++)
                platforms[i].move_down(player_vel_y);

            player.set_vel_y(player_vel_y - 1);

            if (player_vel_y == 1)
                player.set_vel_y(-1); // avoid floating
        }
        else if (player_vel_y < 0)
        {
            ground.move_up(-player_vel_y);
            background.move_up(-player_vel_y);
            animal.move_up(-player_vel_y);
            ally.move_up(-player_vel_y);

            for (int i = 0; i < num_platforms[level]; i++)
                platforms[i].move_up(-player_vel_y);

            if (player_vel_y > -10) // min vel_y
                player.set_vel_y(player_vel_y - 1);

            if (ground.detect_player_collision(player.get_rect()))
            {
                player.set_vel_y(0);
            }

            bool platform_collision = false;
            for (int i = 0; i < num_platforms[level]; i++)
            {
                if (platforms[i].detect_player_collision(player.get_rect()))
                    platform_collision = true;
            }

            // player got on platform
            if (!player.get_is_on_platform() && platform_collision)
            {
                player.set_vel_y(0);
                player.set_is_on_platform(true);
            }
        }

        bool no_platform_player_collision = true;
        for (int i = 0; i < num_platforms[level]; i++)
        {
            if (platforms[i].detect_player_collision(player.get_rect()))
                no_platform_player_collision = false;
        }

        // player must start falling from platform
        if (player.get_is_on_platform() && no_platform_player_collision)
        {
            // std::cout << "no collision and is on platform\n";
            player.set_is_on_platform(false);
            if (player.get_vel_y() == 0) // if walked down from platform
                player.set_vel_y(-1);
        }

        background.move();
        if (ground.move())
        {
            animal.move();
            ally.move();
            for (int i = 0; i < num_platforms[level]; i++)
                platforms[i].move();
        }
        for (int i = 0; i < (int)enemies.size(); i++)
        {
            enemies[i].move();
        }

        background.draw();
        ground.draw();
        for (int i = 0; i < num_platforms[level]; i++)
        {
            platforms[i].draw();
        }
        if (animals_left > 0)
            animal.draw();
        ally.draw();
        for (int i = 0; i < (int)enemies.size(); i++)
        {
            SDL_Rect tmp_rect = platforms[enemies[i].get_platform_num()].get_rect();
            enemies[i].draw(tmp_rect.x, tmp_rect.y);
        }

        player.draw();

        // calculate arrow direction
        SDL_Rect tmp_an_rect = animal.get_rect();
        if (tmp_an_rect.y < 0)
            arrow.set_direction(1);
        else if (tmp_an_rect.y > HEIGHT - ANIMAL_HEIGHT)
            arrow.set_direction(2);
        else if (tmp_an_rect.x < 0)
            arrow.set_direction(3);
        else if (tmp_an_rect.x > WIDTH - ANIMAL_WIDTH)
            arrow.set_direction(4);
        else
            arrow.set_direction(0);

        if (animals_left > 0)
            arrow.draw();

        // player - enemy collision
        SDL_Rect tmp_pl_rect = player.get_rect();
        for (int i = 0; i < (int)enemies.size(); i++)
        {
            if (enemies[i].detect_player_collision(tmp_pl_rect))
                health.decrease();
        }
        health.draw();

        // player - animal collision
        if (animal.detect_player_collision(tmp_pl_rect) && animals_left > 0)
        {
            // std::cout << "player-animal collision\n";
            int tmp = rand() % num_platforms[level];
            // std::cout << "plat: " << tmp << std::endl;

            SDL_Rect tmp_rect = platforms[tmp].get_rect();
            int tmp_relative_x = rand() % (PLAT_WIDTH - ANIMAL_WIDTH);
            animal.reset(tmp_rect.x + tmp_relative_x, tmp_rect.y - ANIMAL_HEIGHT + 3);

            // decrease animals left
            animals_left--;
            player_stats.points++;
            char tmp_text[40] = "Preostale zivali:  ";
            tmp_text[strlen(tmp_text) - 1] = '0' + animals_left;
            preostale.change_text(tmp_text);
        }

        // player - ally collision
        if (ally.detect_player_collision(tmp_pl_rect))
        {
            // std::cout << "player-ally collision\n";
            // std::cout << "plat: " << tmp << std::endl;

            if (health.get_health() < MAX_HEALTH) // can't have more than max health
            {
                int tmp = rand() % num_platforms[level];
                SDL_Rect tmp_rect = platforms[tmp].get_rect();
                int tmp_relative_x = rand() % (PLAT_WIDTH - PL_WIDTH);

                ally.reset(tmp_rect.x + tmp_relative_x, tmp_rect.y - PL_HEIGHT + 3);

                health.increase();
            }
        }

        preostale.draw();
        level_curr.draw();

        if (animals_left <= 0 && level == 3) // game is finished
        {
            game_over_screen.won(); // just change text of game over screen
            screen = 1;
            just_died = true; // for screen to work correctly
        }
        // if level is finished
        else if (animals_left <= 0)
        {
            // std::cout << "Stopnja je opravljena";
            curr_time = std::chrono::steady_clock::now();

            if (first_finish)
            {
                finish_time = curr_time;
                first_finish = false;
            }

            level_done.draw();

            if ((curr_time - finish_time).count() > 2000000000) // 2 s
            {
                level++;
                first_finish = true;

                this->init_level();
            }
        }

        // if player is dead and hasn't finished the level
        if (health.get_health() <= 0 && first_finish == true)
        {
            game_over_screen.reset();
            screen = 1;
            just_died = true;
        }

        // update time counter
        curr_game_time = std::chrono::steady_clock::now();

        std::chrono::duration<int> time_span_game = std::chrono::duration_cast<std::chrono::duration<int>>(curr_game_time - start_game_time);

        char tmp_text[40];
        sprintf(tmp_text, "%d", time_span_game.count() + add_play_time);
        char tmp_text2[50] = "Cas: ";
        strcat(tmp_text2, tmp_text);
        strcat(tmp_text2, " s");
        time_text.change_text(tmp_text2, 20);

        time_text.draw();

        // frame is updated
        last_frame = curr_time;
        SDL_RenderPresent(renderer);
    }
}

void Game::game_over()
{
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0)
    {
        // std::cout << e.type << std::endl;
        if (e.type == SDL_QUIT)
            is_running = false;
        else if (e.type == SDL_KEYDOWN)
        {
            switch (e.key.keysym.sym)
            {
            case SDLK_n:
                game_over_screen.reset(); // reset game over text
                screen = 0;
                level = 0;

                this->init_level();
                break;
            case SDLK_m:
                game_over_screen.reset(); // reset game over text
                screen = 2;
                break;
            }
        }
    }

    if (just_died) // to draw only once
    {
        curr_game_time = std::chrono::steady_clock::now();
        std::chrono::duration<int> time_span = std::chrono::duration_cast<std::chrono::duration<int>>(curr_game_time - start_game_time);
        player_stats.time = time_span.count() + add_play_time;

        write_result();
        game_over_screen.load_data();
        // std::ifstream datai("results.bin", std::ios::binary);
        // struct PlayerStats tmp;

        // while (datai.read((char *)&tmp, sizeof(tmp)))
        // {
        //     std::cout << tmp.name << " " << tmp.points << " " << tmp.time << std::endl;
        // }
        // std::cout << "\n";
        // datai.close();

        game_over_screen.draw();
        just_died = false;
        SDL_RenderPresent(renderer);
    }
}

void Game::menu()
{
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0)
    {
        // std::cout << e.type << std::endl;
        if (e.type == SDL_QUIT)
            is_running = false;
        else if (e.type == SDL_KEYDOWN)
        {
            switch (e.key.keysym.sym)
            {
            case SDLK_j:
            case SDLK_DOWN:
            case SDLK_s:
                menu_screen.move_down();
                break;
            case SDLK_k:
            case SDLK_UP:
            case SDLK_w:
                menu_screen.move_up();
                break;
            case SDLK_SPACE:
            case SDLK_RETURN:
                int pos = menu_screen.get_position();
                if (pos == 0)
                {
                    screen = 0;
                    level = 0;
                    char tmp_text[50] = "Vpisi svoje ime v konzolo";
                    menu_screen.change_title(tmp_text);

                    SDL_RenderClear(renderer);
                    menu_screen.draw();
                    SDL_RenderPresent(renderer);

                    std::cout << "Vpisi svoje ime: ";
                    do
                        gets(player_stats.name);
                    while (strlen(player_stats.name) > 20);

                    strcpy(tmp_text, "Osvoboditelj");
                    menu_screen.change_title(tmp_text);
                    this->init_level();
                }
                else if (pos == 1)
                    this->load_game();
                else if (pos == 2)
                    screen = 3;
                else if (pos == 3)
                    this->replay();
                else
                    is_running = false;
                break;
            }
        }
    }

    SDL_RenderClear(renderer);
    menu_screen.draw();
    SDL_RenderPresent(renderer);
}

void Game::replay()
{

    std::ifstream datai_replay("replay.bin", std::ios::binary);
    int tmp;
    if (datai_replay.is_open())
    {
        // --- start init ---
        int start_x;
        datai_replay.read((char *)&start_x, sizeof(int));
        // std::cout << start_x << std::endl;

        // level = 3;

        // std::cout << "Start init\n";

        animals_left = num_animals[level];
        just_died = false;

        // create text for preostale
        char tmp_text[40] = "Preostale zivali:  ";
        tmp_text[strlen(tmp_text) - 1] = '0' + animals_left;
        preostale.init(renderer, 20, 10, tmp_text);

        // create text for level_curr
        strcpy(tmp_text, "Stopnja   od 4");
        tmp_text[8] = '1' + level;
        // std::cout << tmp_text << std::endl;
        level_curr.init(renderer, 20, 50, tmp_text);

        strcpy(tmp_text, "0 s");
        time_text.init(renderer, 20, 90, tmp_text);

        background.reset(start_x);
        ground.reset(start_x);

        enemies.clear();
        for (int i = 0; i < num_platforms[level]; i++)
        {
            bool is_on_plat;
            datai_replay.read((char *)&is_on_plat, sizeof(is_on_plat));
            if (is_on_plat)
            {
                struct EnemyStats tmp_enemy;
                datai_replay.read((char *)&tmp_enemy, sizeof(tmp_enemy));

                Enemy enemy;
                enemies.push_back(enemy);
                enemies[enemies.size() - 1].init(renderer, i, tmp_enemy);
            }
        }

        // std::cout << "Mid init\n";
        // platforms.clear();

        // prva platforma je max polovico zaslona levo ali cetrtino zaslona desno od igralca
        int plat_x, plat_y;

        for (int i = 0; i < num_platforms[level]; i++)
        {
            datai_replay.read((char *)&plat_x, sizeof(int));
            datai_replay.read((char *)&plat_y, sizeof(int));
            // std::cout << plat_x << " " << plat_y << std::endl;
            platforms[i].reset(plat_x, plat_y);
        }

        // first animal position
        struct AnimalStats tmp_animal;
        datai_replay.read((char *)&tmp_animal, sizeof(tmp_animal));
        animal.reset(tmp_animal.x, tmp_animal.y);

        // first ally position
        struct AllyStats tmp_ally;
        datai_replay.read((char *)&tmp_ally, sizeof(tmp_ally));
        ally.reset(tmp_ally.x, tmp_ally.y);

        // resetting values
        ground.set_direction(0);
        background.set_direction(0);
        player.set_direction(0);
        for (int i = 0; i < num_platforms[level]; i++)
            platforms[i].set_direction(0);
        animal.set_direction(0);
        ally.set_direction(0);

        health.reset();
        player.set_vel_y(-10);

        if (level == 0)
        {
            start_game_time = std::chrono::steady_clock::now();
        }

        last_move_time = std::chrono::steady_clock::now();

        // --- finish init ---

        // --- start replay gameplay ---

        while (datai_replay.read((char *)&tmp, sizeof(int)))
        {
            std::cout << "until next move: " << tmp << std::endl;
            curr_time = std::chrono::steady_clock::now();
            int time_span_move = (curr_time - last_move_time).count();
            int direction;

            while (time_span_move < tmp)
            {
                if ((curr_time - last_frame).count() > 10000000) // > 10 ms, 100 Hz
                {
                    SDL_RenderClear(renderer);

                    int player_vel_y = player.get_vel_y();

                    if (player_vel_y > 0)
                    {
                        ground.move_down(player_vel_y);
                        background.move_down(player_vel_y);
                        animal.move_down(player_vel_y);
                        ally.move_down(player_vel_y);

                        for (int i = 0; i < num_platforms[level]; i++)
                            platforms[i].move_down(player_vel_y);

                        player.set_vel_y(player_vel_y - 1);

                        if (player_vel_y == 1)
                            player.set_vel_y(-1); // avoid floating
                    }
                    else if (player_vel_y < 0)
                    {
                        ground.move_up(-player_vel_y);
                        background.move_up(-player_vel_y);
                        animal.move_up(-player_vel_y);
                        ally.move_up(-player_vel_y);

                        for (int i = 0; i < num_platforms[level]; i++)
                            platforms[i].move_up(-player_vel_y);

                        if (player_vel_y > -10) // min vel_y
                            player.set_vel_y(player_vel_y - 1);

                        if (ground.detect_player_collision(player.get_rect()))
                        {
                            player.set_vel_y(0);
                        }

                        bool platform_collision = false;
                        for (int i = 0; i < num_platforms[level]; i++)
                        {
                            if (platforms[i].detect_player_collision(player.get_rect()))
                                platform_collision = true;
                        }

                        // player got on platform
                        if (!player.get_is_on_platform() && platform_collision)
                        {
                            player.set_vel_y(0);
                            player.set_is_on_platform(true);
                        }
                    }

                    bool no_platform_player_collision = true;
                    for (int i = 0; i < num_platforms[level]; i++)
                    {
                        if (platforms[i].detect_player_collision(player.get_rect()))
                            no_platform_player_collision = false;
                    }

                    // player must start falling from platform
                    if (player.get_is_on_platform() && no_platform_player_collision)
                    {
                        // std::cout << "no collision and is on platform\n";
                        player.set_is_on_platform(false);
                        if (player.get_vel_y() == 0) // if walked down from platform
                            player.set_vel_y(-1);
                    }

                    background.move();
                    if (ground.move())
                    {
                        animal.move();
                        ally.move();
                        for (int i = 0; i < num_platforms[level]; i++)
                            platforms[i].move();
                    }
                    for (int i = 0; i < (int)enemies.size(); i++)
                    {
                        enemies[i].move();
                    }

                    background.draw();
                    ground.draw();
                    for (int i = 0; i < num_platforms[level]; i++)
                    {
                        platforms[i].draw();
                    }
                    if (animals_left > 0)
                        animal.draw();
                    ally.draw();
                    for (int i = 0; i < (int)enemies.size(); i++)
                    {
                        SDL_Rect tmp_rect = platforms[enemies[i].get_platform_num()].get_rect();
                        enemies[i].draw(tmp_rect.x, tmp_rect.y);
                    }

                    player.draw();

                    // calculate arrow direction
                    SDL_Rect tmp_an_rect = animal.get_rect();
                    if (tmp_an_rect.y < 0)
                        arrow.set_direction(1);
                    else if (tmp_an_rect.y > HEIGHT - ANIMAL_HEIGHT)
                        arrow.set_direction(2);
                    else if (tmp_an_rect.x < 0)
                        arrow.set_direction(3);
                    else if (tmp_an_rect.x > WIDTH - ANIMAL_WIDTH)
                        arrow.set_direction(4);
                    else
                        arrow.set_direction(0);

                    if (animals_left > 0)
                        arrow.draw();

                    // player - enemy collision
                    SDL_Rect tmp_pl_rect = player.get_rect();
                    for (int i = 0; i < (int)enemies.size(); i++)
                    {
                        if (enemies[i].detect_player_collision(tmp_pl_rect))
                            health.decrease();
                    }
                    health.draw();

                    // player - animal collision
                    if (animal.detect_player_collision(tmp_pl_rect) && animals_left > 0)
                    {
                        // std::cout << "player-animal collision\n";
                        int tmp = rand() % num_platforms[level];
                        // std::cout << "plat: " << tmp << std::endl;

                        SDL_Rect tmp_rect = platforms[tmp].get_rect();
                        int tmp_relative_x = rand() % (PLAT_WIDTH - ANIMAL_WIDTH);
                        animal.reset(tmp_rect.x + tmp_relative_x, tmp_rect.y - ANIMAL_HEIGHT + 3);

                        // decrease animals left
                        animals_left--;
                        player_stats.points++;
                        char tmp_text[40] = "Preostale zivali:  ";
                        tmp_text[strlen(tmp_text) - 1] = '0' + animals_left;
                        preostale.change_text(tmp_text);
                    }

                    // player - ally collision
                    if (ally.detect_player_collision(tmp_pl_rect))
                    {
                        // std::cout << "player-ally collision\n";
                        // std::cout << "plat: " << tmp << std::endl;

                        if (health.get_health() < MAX_HEALTH) // can't have more than max health
                        {
                            int tmp = rand() % num_platforms[level];
                            SDL_Rect tmp_rect = platforms[tmp].get_rect();
                            int tmp_relative_x = rand() % (PLAT_WIDTH - PL_WIDTH);

                            ally.reset(tmp_rect.x + tmp_relative_x, tmp_rect.y - PL_HEIGHT + 3);

                            health.increase();
                        }
                    }

                    preostale.draw();
                    level_curr.draw();

                    if (animals_left <= 0 && level == 3) // game is finished
                    {
                        game_over_screen.won(); // just change text of game over screen
                        screen = 1;
                        just_died = true; // for screen to work correctly
                    }
                    // if level is finished
                    else if (animals_left <= 0)
                    {
                        // std::cout << "Stopnja je opravljena";
                        curr_time = std::chrono::steady_clock::now();

                        if (first_finish)
                        {
                            finish_time = curr_time;
                            first_finish = false;
                        }

                        level_done.draw();

                        if ((curr_time - finish_time).count() > 2000000000) // 2 s
                        {
                            level++;
                            first_finish = true;

                            std::cout << "Narisi naslednji level\n";

                            // this->init_level();
                        }
                    }

                    // if player is dead and hasn't finished the level
                    if (health.get_health() <= 0 && first_finish == true)
                    {
                        screen = 2;
                        just_died = true;
                    }

                    // update time counter
                    curr_game_time = std::chrono::steady_clock::now();

                    std::chrono::duration<int> time_span_game = std::chrono::duration_cast<std::chrono::duration<int>>(curr_game_time - start_game_time);

                    char tmp_text[40];
                    sprintf(tmp_text, "%d", time_span_game.count());
                    char tmp_text2[50] = "Cas: ";
                    strcat(tmp_text2, tmp_text);
                    strcat(tmp_text2, " s");
                    time_text.change_text(tmp_text2, 20);

                    time_text.draw();

                    // frame is updated
                    last_frame = curr_time;
                    SDL_RenderPresent(renderer);
                }

                curr_time = std::chrono::steady_clock::now();
                time_span_move = (curr_time - last_move_time).count();
            }

            // when next move is ready
            datai_replay.read((char *)&direction, sizeof(int));
            // std::cout << "next move: " << direction << std::endl;
            switch (direction)
            {
            case 1:
                last_move_time = std::chrono::steady_clock::now();

                ground.set_direction(1);
                background.set_direction(1);
                player.set_direction(1);
                for (int i = 0; i < num_platforms[level]; i++)
                    platforms[i].set_direction(1);
                animal.set_direction(1);
                ally.set_direction(1);

                break;
            case 2:
                last_move_time = std::chrono::steady_clock::now();

                ground.set_direction(2);
                background.set_direction(2);
                player.set_direction(2);
                for (int i = 0; i < num_platforms[level]; i++)
                    platforms[i].set_direction(2);
                animal.set_direction(2);
                ally.set_direction(2);

                break;
            case 0:
                last_move_time = std::chrono::steady_clock::now();

                ground.set_direction(0);
                background.set_direction(0);
                player.set_direction(0);
                for (int i = 0; i < num_platforms[level]; i++)
                    platforms[i].set_direction(0);
                animal.set_direction(0);
                ally.set_direction(0);

                break;
            case 3:
                last_move_time = std::chrono::steady_clock::now();

                // std::cout << "space " << player.get_vel_y() << std::endl;
                if (player.get_vel_y() == 0) // no double jumping
                    player.set_vel_y(20);

                break;
            }
        }

        // --- finish replay gameplay ---

        datai_replay.close();
    }
}

void Game::tutorial()
{
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0)
    {
        // std::cout << e.type << std::endl;
        if (e.type == SDL_QUIT)
            is_running = false;
        else if (e.type == SDL_KEYDOWN)
        {
            switch (e.key.keysym.sym)
            {
            case SDLK_SPACE:
            case SDLK_RETURN:
                screen = 2;
                break;
            }
        }
    }

    SDL_RenderClear(renderer);
    tutorial_screen.draw();
    SDL_RenderPresent(renderer);
}

void Game::write_result()
{
    std::ifstream datai("results.bin", std::ios::binary);
    std::ofstream datao("tmp.bin", std::ios::binary);
    struct PlayerStats tmp;
    int cnt = 0;
    bool written = false;

    while (datai.read((char *)&tmp, sizeof(tmp)))
    {
        if (cnt == 5)
            break;

        cnt++;

        if (tmp.points > player_stats.points)
            datao.write((char *)&tmp, sizeof(tmp));
        else if (tmp.points == player_stats.points)
        {
            if (tmp.time <= player_stats.time)
                datao.write((char *)&tmp, sizeof(tmp));
            else
            {
                datao.write((char *)&player_stats, sizeof(player_stats));
                written = true;
                break;
            }
        }
        else
        {
            datao.write((char *)&player_stats, sizeof(player_stats));
            written = true;
            break;
        }
    }

    if (cnt < 5 && !written) // so bili zapisani vsi razen trenutnega in je se prostor
        datao.write((char *)&player_stats, sizeof(player_stats));
    else if (cnt < 5 && written) // trenutni je bil ze vpisan in imamo se prostor
    {
        datao.write((char *)&tmp, sizeof(tmp)); // ga se nismo zapisali
        cnt++;

        while (datai.read((char *)&tmp, sizeof(tmp))) // zapisuj dokler se lahko
        {
            if (cnt == 5)
                break;

            cnt++;
            datao.write((char *)&tmp, sizeof(tmp));
        }
    }

    datai.close();
    datao.close();

    remove("results.bin");
    rename("tmp.bin", "results.bin");
}

void Game::save_game()
{
    struct GameStats game_stats;

    // check if game was previously saved
    bool was_saved_before = false;
    std::ifstream datai_save("saved.bin", std::ios::binary);

    if (datai_save.is_open())
        if (datai_save.read((char *)&game_stats, sizeof(game_stats)))
            was_saved_before = true;

    datai_save.close();

    std::ofstream datao_save("saved.bin", std::ios::binary);

    game_stats.level = level;

    std::chrono::duration<int> time_span_game = std::chrono::duration_cast<std::chrono::duration<int>>(curr_game_time - start_game_time);
    if (was_saved_before)
        game_stats.play_time += time_span_game.count();
    else
        game_stats.play_time = time_span_game.count();

    game_stats.animals_left = animals_left;
    game_stats.health = health.get_health();

    SDL_Rect tmp_rect = animal.get_rect();

    game_stats.animal_x = tmp_rect.x;
    game_stats.animal_y = tmp_rect.y;

    tmp_rect = ally.get_rect();
    game_stats.ally_x = tmp_rect.x;
    game_stats.ally_y = tmp_rect.y;

    tmp_rect = ground.get_rect();
    game_stats.ground_x = tmp_rect.x;
    game_stats.ground_y = tmp_rect.y;

    tmp_rect = background.get_rect();
    game_stats.background_y = tmp_rect.y;

    for (int i = 0; i < num_platforms[level]; i++)
    {
        tmp_rect = platforms[i].get_rect();
        game_stats.platforms_x[i] = tmp_rect.x;
        game_stats.platforms_y[i] = tmp_rect.y;
    }

    game_stats.num_enemies = enemies.size();
    for (int i = 0; i < (int)enemies.size(); i++)
    {
        game_stats.enemies_platforms[i] = enemies[i].get_platform_num();
        game_stats.enemies_directions[i] = enemies[i].get_direction();
        game_stats.enemies_x[i] = enemies[i].get_relative_x();
    }

    game_stats.player_stats = player_stats;

    datao_save.write((char *)&game_stats, sizeof(game_stats));

    datao_save.close();
}

void Game::load_game()
{
    std::ifstream datai_save("saved.bin", std::ios::binary);
    struct GameStats game_stats;

    if (datai_save.is_open())
    {
        if (!datai_save.read((char *)&game_stats, sizeof(game_stats))) // if file is empty
        {
            return;
        }

        level = game_stats.level;
        add_play_time = game_stats.play_time;
        animals_left = game_stats.animals_left;

        health.reset();
        for (int i = 0; i < MAX_HEALTH - game_stats.health; i++)
            health.decrease();

        animal.reset(game_stats.animal_x, game_stats.animal_y);
        ally.reset(game_stats.ally_x, game_stats.ally_y);

        for (int i = 0; i < num_platforms[level]; i++)
        {
            platforms[i].reset(game_stats.platforms_x[i], game_stats.platforms_y[i]);
        }

        enemies.clear();
        for (int i = 0; i < game_stats.num_enemies; i++)
        {
            Enemy enemy;
            enemy.init(renderer, game_stats.enemies_platforms[i]);
            enemy.reset(game_stats.enemies_directions[i], game_stats.enemies_x[i]);
            enemies.push_back(enemy);
        }

        player_stats = game_stats.player_stats;

        // create text for preostale
        char tmp_text[40] = "Preostale zivali:  ";
        tmp_text[strlen(tmp_text) - 1] = '0' + animals_left;
        preostale.init(renderer, 20, 10, tmp_text);

        // create text for level_curr
        strcpy(tmp_text, "Stopnja   od 4");
        tmp_text[8] = '1' + level;
        // std::cout << tmp_text << std::endl;
        level_curr.init(renderer, 20, 50, tmp_text);

        strcpy(tmp_text, "0 s");
        time_text.init(renderer, 20, 90, tmp_text);

        background.reset(game_stats.ground_x, game_stats.background_y);
        ground.reset(game_stats.ground_x, game_stats.ground_y);

        // resetting values
        ground.set_direction(0);
        background.set_direction(0);
        player.set_direction(0);
        for (int i = 0; i < num_platforms[level]; i++)
            platforms[i].set_direction(0);
        animal.set_direction(0);
        ally.set_direction(0);

        last_move_time = std::chrono::steady_clock::now();
        start_game_time = std::chrono::steady_clock::now();

        just_died = false;
        screen = 0;
    }

    datai_save.close();
}