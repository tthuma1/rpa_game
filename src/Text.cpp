#include <Text.h>
#include <Config.h>
#include <iostream>

void Text::init(SDL_Renderer *renderer, int x, int y, char text[])
{
    // std::cout << "Text init\n";
    this->renderer = renderer;

    // std::cout << text << std::endl;

    font = TTF_OpenFont("assets/Arcadepix Plus.ttf", 40);
    color = {255, 255, 255};

    SDL_Surface *surfaceMessage = TTF_RenderText_Solid(font, text, color);

    message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
    messageRect = {WIDTH - surfaceMessage->w - x, y, surfaceMessage->w, surfaceMessage->h};

    SDL_RenderCopy(this->renderer, message, NULL, &messageRect);
}

void Text::init_center(SDL_Renderer *renderer, int y, int size, char text[])
{
    // std::cout << "Text init center\n";
    this->renderer = renderer;

    // std::cout << text << std::endl;

    font = TTF_OpenFont("assets/Arcadepix Plus.ttf", size);
    color = {255, 255, 255};

    SDL_Surface *surfaceMessage = TTF_RenderText_Solid(font, text, color);

    message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
    int x = (WIDTH - surfaceMessage->w) / 2;
    messageRect = {x, y, surfaceMessage->w, surfaceMessage->h};

    SDL_RenderCopy(this->renderer, message, NULL, &messageRect);
}

void Text::draw()
{
    SDL_RenderCopy(renderer, message, NULL, &messageRect);
}

void Text::change_text(char text[])
{
    SDL_Surface *surfaceMessage = TTF_RenderText_Solid(font, text, color);
    message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
    messageRect.w = surfaceMessage->w;
}

void Text::change_text(char text[], int x)
{
    SDL_Surface *surfaceMessage = TTF_RenderText_Solid(font, text, color);
    message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
    messageRect.w = surfaceMessage->w;
    messageRect.x = WIDTH - surfaceMessage->w - x;
}

void Text::change_text_center(char text[])
{
    SDL_Surface *surfaceMessage = TTF_RenderText_Solid(font, text, color);
    message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
    messageRect.w = surfaceMessage->w;

    int x = (WIDTH - surfaceMessage->w) / 2;
    messageRect.x = x;
}