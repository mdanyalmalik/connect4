#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

#include "RenderWindow.hpp"
#include "Entity.hpp"

using namespace std;

RenderWindow::RenderWindow(const char *p_title, int p_w, int p_h) : window(NULL), renderer(NULL)
{
    window = SDL_CreateWindow(p_title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, p_w, p_h, SDL_WINDOW_SHOWN);

    SDL_Surface *icon = IMG_Load("res/gfx/icon.png");
    SDL_SetWindowIcon(window, icon);

    if (window == NULL)
        cout << "Window failed to init";

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC || SDL_RENDERER_ACCELERATED); // second is graphics driver

    if (renderer == NULL)
        cout << "Renderer failed to init";
}

SDL_Texture *RenderWindow::loadTexture(const char *p_filepath)
{
    SDL_Texture *texture = NULL;
    texture = IMG_LoadTexture(renderer, p_filepath);

    if (texture == NULL)
        cout << "Failed to load texture" << endl;

    return texture;
}

void RenderWindow::drawRect(int p_x, int p_y, int p_w, int p_h)
{
    SDL_Rect rect;

    rect.x = p_x;
    rect.y = p_y;
    rect.w = p_w;
    rect.h = p_h;

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 75);
    SDL_RenderFillRect(renderer, &rect);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
}

void RenderWindow::cleanUp()
{
    SDL_DestroyWindow(window);
}

void RenderWindow::clear()
{
    SDL_RenderClear(renderer);
}

void RenderWindow::render(Entity &p_entity)
{
    SDL_Rect src;
    src.x = p_entity.getCurrentFrame().x;
    src.y = p_entity.getCurrentFrame().y;
    src.w = p_entity.getCurrentFrame().w;
    src.h = p_entity.getCurrentFrame().h;

    SDL_Rect dst;
    dst.x = p_entity.getX();
    dst.y = p_entity.getY();
    dst.w = p_entity.getCurrentFrame().w;
    dst.h = p_entity.getCurrentFrame().h;

    SDL_RenderCopy(renderer, p_entity.getTex(), &src, &dst); // last two pointers are viewable portion of texture
}

void RenderWindow::display()
{
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    SDL_RenderPresent(renderer);
}