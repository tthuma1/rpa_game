#include <Tutorial.h>
#include <Config.h>
#include <iostream>

void Tutorial::init(SDL_Renderer *renderer)
{
    this->renderer = renderer;

    // background
    rect_bg = {-500, -BG_HEIGHT + HEIGHT + 100, BG_WIDTH, BG_HEIGHT};
    SDL_Surface *surface = IMG_Load("assets/background.png");
    background = SDL_CreateTextureFromSurface(this->renderer, surface);

    // std::cout << "Navdila init\n";

    // title
    char tmp_text[60] = "Navodila";
    title.init_center(renderer, 10, 50, tmp_text);

    // std::cout << "Navodila title initialized\n";

    // besedila
    int i = 0;
    for (; i < 2; i++)
    {
        strcpy(tmp_text, tutorial[i]);
        texts[i].init_center(renderer, 80 + i * 30, 30, tmp_text);
    }

    for (; i < 8; i++)
    {
        strcpy(tmp_text, tutorial[i]);
        texts[i].init_center(renderer, 160 + (i - 2) * 30, 30, tmp_text);
    }

    for (; i < 13; i++)
    {
        strcpy(tmp_text, tutorial[i]);
        texts[i].init_center(renderer, 360 + (i - 8) * 30, 30, tmp_text);
    }

    strcpy(tmp_text, tutorial[i]);
    texts[i].init_center(renderer, 540, 40, tmp_text);

    // arrow za izhod
    rect_arrow_choice = {WIDTH - 450, 540 - 5, 75, 50};
    char tmp[40] = "assets/arrow/menu_arrow.png";

    // std::cout << tmp << std::endl;
    surface = IMG_Load(tmp);
    arrow_choice = SDL_CreateTextureFromSurface(this->renderer, surface);

    // player
    player.init(renderer);
    player.reset(WIDTH - 170, 90);

    // animal
    animal.init(renderer);
    animal.reset(WIDTH - 130, 105);

    // enemy
    enemy.init(renderer, 0);
    enemy.reset();

    // ally
    ally.init(renderer);
    ally.reset(WIDTH - 260, 415);

    // arrow game
    arrow_game.init(renderer);
}

void Tutorial::draw()
{
    SDL_RenderCopy(this->renderer, background, NULL, &rect_bg);

    // besedila
    title.draw();
    for (int i = 0; i < n; i++)
        texts[i].draw();

    SDL_RenderCopy(this->renderer, arrow_choice, NULL, &rect_arrow_choice);

    player.draw();
    animal.draw();
    enemy.draw(WIDTH - 200, 420);
    ally.draw();
    arrow_game.draw(WIDTH - 150, 470, 50, 50);
}