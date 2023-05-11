#ifndef SDL_IMAGE_H
#define SDL_IMAGE_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <string>

namespace sdl1{
namespace image{

    void Initiate();
    void DrawToScreen(SDL_Surface* surface, int x, int y);
    SDL_Surface* LoadSprite(const std::string& filename);
    SDL_Surface* NewSpriteScale(SDL_Surface* surface, int newWidth, int newHeight);
    SDL_Surface* NewSpriteColor(SDL_Surface* surface, Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha);
    SDL_Surface* NewSpriteRotation(SDL_Surface* surface, double angle);
    void UpdateScreen();

}
}

#endif