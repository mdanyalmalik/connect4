#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include "Text.hpp"
#include "RenderWindow.hpp"

Text::Text(RenderWindow window, const char *p_fontpath, int font_size, const char *text, const SDL_Color color)
{
    text_texture = loadFont(window, p_fontpath, font_size, text, color);
    SDL_QueryTexture(text_texture, NULL, NULL, &text_rec.w, &text_rec.h);
}

void Text::display(RenderWindow window, int x, int y)
{
    text_rec.x = x;
    text_rec.y = y;

    SDL_RenderCopy(window.renderer, text_texture, NULL, &text_rec);
}

SDL_Texture *Text::loadFont(RenderWindow window, const char *p_fontpath, int font_size, const char *text, const SDL_Color color)
{
    TTF_Font *font = TTF_OpenFont(p_fontpath, font_size);
    if (!font)
    {
        std::cout << "Font failed to open" << std::endl;
    }

    SDL_Surface *text_surface = TTF_RenderText_Solid(font, text, color);
    if (!text_surface)
    {
        std::cout << "Text Surface failed" << std::endl;
    }

    SDL_Texture *text_texture = SDL_CreateTextureFromSurface(window.renderer, text_surface);

    SDL_FreeSurface(text_surface);
    return text_texture;
}

SDL_Rect Text::getTextRect()
{
    return text_rec;
}