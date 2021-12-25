#pragma once // making sure its computed once

#include <SDL.h>
#include <SDL_image.h>
#include "Entity.hpp"

class RenderWindow
{
public:
    RenderWindow(const char *p_title, int p_w, int p_h);
    SDL_Texture *loadTexture(const char *p_filepath);  // returns texture from given file
    void drawRect(int p_x, int p_y, int p_w, int p_h); // draws white rect with low opacity
    void cleanUp();                                    // destroys window
    void clear();                                      // clears renderer
    void render(Entity &p_entity);                     // renders entity object
    void display();                                    // displays rendered textures

    SDL_Window *window;
    SDL_Renderer *renderer;
};