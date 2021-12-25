#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

class Entity
{
public:
    Entity(double p_x, double p_y, double p_w, double p_h, SDL_Texture *p_tex, int floor);
    double getX();                    // gets x coord
    double getY();                    // gets y coord
    void setX(int new_x);             // sets x coord
    void setY(int new_y);             // sets y coord
    void fall(Mix_Chunk *fall_sound); // fall animation, plays sound at bottom
    SDL_Texture *getTex();            // returns texture
    SDL_Rect getCurrentFrame();       // returns rect

private:
    double x, y;
    double a = 0.5;
    double vy = 0;
    double k = 1;
    bool on_floor = false;
    int floor_height;
    SDL_Rect currentFrame;
    SDL_Texture *tex;
};