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
    void NewSpriteScale(const std::string& original, const std::string& label, int newWidth, int newHeight);
    void NewSpriteColor(const std::string& original, const std::string& label, int red, int green, int blue, int alpha);
    void NewSpriteRotation(const std::string& original, const std::string& label, double angle);
    void ClearSprite(const std::string& label);
    void UpdateScreen();

}
}

#endif