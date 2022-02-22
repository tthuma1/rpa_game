#include <GameOver.h>
#include <Config.h>

void GameOver::init(SDL_Renderer *renderer)
{
    this->renderer = renderer;

    char tmp_text[60] = "Konec igre";
    text1.init_center(renderer, HEIGHT / 2 - 100, 50, tmp_text);

    strcpy(tmp_text, "Pritisni N za novo igro");
    text2.init_center(renderer, HEIGHT - 200, 40, tmp_text);

    strcpy(tmp_text, "Pritisni M za zacetni menu");
    text3.init_center(renderer, HEIGHT - 150, 40, tmp_text);
}

void GameOver::draw()
{
    text1.draw();
    text2.draw();
    text3.draw();
}

void GameOver::won()
{
    char tmp_text[] = "Resil si vse zivali!";
    text1.change_text_center(tmp_text);
}

void GameOver::reset()
{
    char tmp_text[] = "Konec igre";
    text1.change_text_center(tmp_text);
}