#ifndef SDL_IMAGE_H
#define SDL_IMAGE_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <string>

namespace sdl1{
namespace image{

    void Initiate();
    void DrawToScreen(const std::string& original, int x, int y);
    void LoadSprite(const std::string& filename, const std::string& label);
    void CreateText(const std::string& label, const std::string& text, const std::string& fontFile, int fontSize, int r, int g, int b);
    void CreateBox(const std::string& label, int width, int height, int r, int g, int b);
    void StitchSprite(const std::string& sprite, const std::string& destination, const std::string& label, int x ,int y);
    void NewSpriteScale(const std::string& original, const std::string& label, int newWidth, int newHeight);
    void NewSpriteColor(const std::string& original, const std::string& label, int red, int green, int blue, int alpha);
    void NewSpriteRotation(const std::string& original, const std::string& label, double angle);
    void ClearSprite(const std::string& label);
    void UpdateScreen();
    void FillRectWithAlpha(SDL_Surface* surface, SDL_Rect rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
}
}

#endif