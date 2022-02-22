#include <iostream>
#include <SDL2/SDL.h>
#include <Game.h>

int main(int argv, char **args)
{
    // std::cout << "starting\n";
    Game game;
    game.run();
    return 0;
}