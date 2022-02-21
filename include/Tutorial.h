#pragma once

#include <SDL2/SDL.h>
#include <Background.h>
#include <Text.h>
#include <Player.h>
#include <Ally.h>
#include <Enemy.h>
#include <Animal.h>
#include <Arrow.h>

class Tutorial
{
private:
    SDL_Renderer *renderer;

    SDL_Texture *background, *arrow_choice;

    SDL_Rect rect_bg, rect_arrow_choice;

    Player player;
    Ally ally;
    Enemy enemy;
    Animal animal;
    Arrow arrow_game;

    Text title, texts[20];
    char tutorial[20][70] =
        {
            "Ker je svet poln idiotov, ki v skritih laboratorijih",
            "zlorabljajo zivali, se mora Erazem boriti proti njim",
            "Po svetu se premikas z:",
            "a = levo",
            "d = desno",
            "w ali presledek = skok",
            "s ali j = ustavi se",
            "Lahko pa se premikas tudi s puscicami na tipkovnici",
            "Na voljo imas 5 zivljenj za vsako stopnjo",
            "Zlobni znanstveniki Vasje ti zivljenje vzamejo",
            "Pomocnica Neza pa ti zivljenje doda",
            "Opraviti moras 4 stopnje",
            "Na zaslonu ti bo puscica povedala v kateri smeri je zival",
            "Nazaj",
    };
    int n = 14; // number of actually used texts, could delete when finished

public:
    void init(SDL_Renderer *renderer);
    void draw();
};