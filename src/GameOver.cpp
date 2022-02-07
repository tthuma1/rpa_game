#include <GameOver.h>
#include <Config.h>

void GameOver::init(SDL_Renderer *renderer)
{
    this->renderer = renderer;

    char tmp_text1[] = "Konec igre";
    text1.init(renderer, 450, HEIGHT / 2 - 100, tmp_text1);

    char tmp_text2[] = "Pritisni N za novo igro";
    text2.init(renderer, 320, HEIGHT - 200, tmp_text2);
}

void GameOver::draw()
{
    text1.draw();
    text2.draw();
}