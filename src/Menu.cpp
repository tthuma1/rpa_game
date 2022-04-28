#include <Menu.h>
#include <Config.h>

void Menu::init(SDL_Renderer *renderer)
{
    this->renderer = renderer;
    position = 0;

    // background
    rect_bg = {-500, -BG_HEIGHT + HEIGHT + 100, BG_WIDTH, BG_HEIGHT};
    SDL_Surface *surface = IMG_Load("assets/background.png");
    background = SDL_CreateTextureFromSurface(this->renderer, surface);

    // title
    char tmp_text[60] = "Osvoboditelj";
    title.init_center(renderer, HEIGHT / 2 - 140, 70, tmp_text);

    // izbira
    strcpy(tmp_text, "Nova igra");
    options[0].init_center(renderer, HEIGHT / 2 + 50, 40, tmp_text);

    strcpy(tmp_text, "Navodila");
    options[1].init_center(renderer, HEIGHT / 2 + 100, 40, tmp_text);

    strcpy(tmp_text, "Izhod");
    options[2].init_center(renderer, HEIGHT / 2 + 150, 40, tmp_text);

    // arrow
    rect_arrow = {WIDTH - 400, HEIGHT / 2 + 50 - 5, 75, 50}; // y start on position 0
    char tmp[40] = "assets/arrow/menu_arrow.png";

    // std::cout << tmp << std::endl;
    surface = IMG_Load(tmp);
    arrow = SDL_CreateTextureFromSurface(this->renderer, surface);
}

void Menu::draw()
{
    SDL_RenderCopy(this->renderer, background, NULL, &rect_bg);
    title.draw();
    for (int i = 0; i < 3; i++)
        options[i].draw();
    SDL_RenderCopy(this->renderer, arrow, NULL, &rect_arrow);
}

void Menu::move_down()
{
    if (position != 2)
    {
        position++;
        rect_arrow.y += 50;
    }
    else
    {
        position = 0;
        rect_arrow.y = HEIGHT / 2 + 50 - 5;
    }
}

void Menu::move_up()
{
    if (position != 0)
    {
        position--;
        rect_arrow.y -= 50;
    }
    else
    {
        position = 2;
        rect_arrow.y = HEIGHT / 2 + 150 - 5;
    }
}

int Menu::get_position()
{
    return position;
}

void Menu::change_title(char text[])
{
    title.change_text_center(text);
}