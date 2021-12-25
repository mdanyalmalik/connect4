#include "Entity.hpp"
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <SDL_mixer.h>

Entity::Entity(double p_x, double p_y, double p_w, double p_h, SDL_Texture *p_tex, int floor)
    : x(p_x), y(p_y), tex(p_tex)
{
    currentFrame.x = 0;
    currentFrame.y = 0;
    currentFrame.w = p_w;
    currentFrame.h = p_h;
    floor_height = floor;
}

double Entity::getX()
{
    return x;
}

double Entity::getY()
{
    return y;
}

void Entity::setX(int new_x)
{
    x = new_x;
}

void Entity::setY(int new_y)
{
    y = new_y;
}

void Entity::fall(Mix_Chunk *fall_sound)
{
    if (floor_height == -1)
        return;
    else if (!on_floor)
    {
        // fall
        if (y <= floor_height - currentFrame.h)
        {
            vy += a;
            y += vy;
        }
        else
        {
            vy = 0;
            Mix_PlayChannel(0, fall_sound, 0);
            y = floor_height - currentFrame.h;
            on_floor = true;
        }
    }
}

SDL_Texture *Entity::getTex()
{
    return tex;
}

SDL_Rect Entity::getCurrentFrame()
{
    return currentFrame;
}