#ifndef ENGINE_IMAGE_H
#define ENGINE_IMAGE_H
#include <string>
namespace engine{
namespace image{
    typedef void (*_drawToScreen)(const std::string&, int, int);
    typedef void (*_loadSprite)(const std::string&, const std::string&);
    typedef void (*_newSpriteScale)(const std::string&, const std::string&, int, int);
    typedef void (*_newSpriteColor)(const std::string&, const std::string&, int, int, int, int);
    typedef void (*_newSpriteRotation)(const std::string&, const std::string&, double);
    typedef void (*_clearSprite)(const std::string&);
    typedef void (*_updateScreen)();
    void driverDrawToScreen(_drawToScreen function);
    void driverLoadSprite(_loadSprite function);
    void driverNewSpriteScale(_newSpriteScale function);
    void driverNewSpriteColor(_newSpriteColor function);
    void driverNewSpriteRotation(_newSpriteRotation function);  
    void driverClearSprite(_clearSprite function);  
    void driverUpdateScreen(_updateScreen function);  
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