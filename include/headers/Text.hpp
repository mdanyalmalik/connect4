#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include "RenderWindow.hpp"

class Text
{
public:
    Text(RenderWindow window, const char *p_fontpath, int font_size, const char *text, const SDL_Color color);
    void display(RenderWindow window, int x, int y);
    SDL_Texture *loadFont(RenderWindow window, const char *p_fontpath, int font_size, const char *text, const SDL_Color color);
    SDL_Rect getTextRect();

private:
    SDL_Texture *text_texture = NULL;
    SDL_Rect text_rec;
};