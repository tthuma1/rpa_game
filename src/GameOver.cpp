#include <GameOver.h>
#include <Config.h>

void GameOver::init(SDL_Renderer *renderer)
{
    this->renderer = renderer;

    char tmp_text[60] = "Konec igre";
    text1.init_center(renderer, 100, 50, tmp_text);

    strcpy(tmp_text, "Pritisni N za novo igro");
    text2.init_center(renderer, HEIGHT - 150, 40, tmp_text);

    strcpy(tmp_text, "Pritisni M za zacetni menu");
    text3.init_center(renderer, HEIGHT - 100, 40, tmp_text);

    load_data();
}

void GameOver::draw()
{
    text1.draw();
    text2.draw();
    text3.draw();
    for (int i = 0; i < 5; i++)
        results[i].draw();
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

void GameOver::load_data()
{
    char tmp_text[60];
    std::ifstream datai("results.bin", std::ios::binary);
    struct PlayerStats tmp;
    int i = 0;
    while (datai.read((char *)&tmp, sizeof(tmp)))
    {

        strcpy(tmp_text, tmp.name);

        char tmp_text2[60], tmp_text3[60];
        sprintf(tmp_text2, "%d", tmp.points);
        sprintf(tmp_text3, "%d", tmp.time);

        strcat(tmp_text, "       ");
        strcat(tmp_text, tmp_text2);
        strcat(tmp_text, " tock       ");
        strcat(tmp_text, tmp_text3);
        strcat(tmp_text, " sekund");

        // std::cout << tmp_text << " " << i << "\n";

        results[i].init_center(renderer, 200 + i * 35, 35, tmp_text);
        i++;
    }

    strcpy(tmp_text, " ");
    for (; i < 5; i++)
    {
        // std::cout << tmp_text << " " << i << "\n";
        results[i].init_center(renderer, 160 + i * 40, 30, tmp_text);
    }
    datai.close();
}