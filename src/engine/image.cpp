
#include "image.h"
namespace engine{
namespace image{
    _drawToScreen _drawToScreenF=nullptr;
    _loadSprite _loadSpriteF=nullptr;
    _newSpriteScale _newSpriteScaleF=nullptr;
    _newSpriteColor _newSpriteColorF=nullptr;
    _newSpriteRotation _newSpriteRotationF=nullptr;
    _clearSprite _clearSpriteF=nullptr;
    _updateScreen _updateScreenF=nullptr;
    void driverDrawToScreen(_drawToScreen function){
        _drawToScreenF=function;
    }
    void driverLoadSprite(_loadSprite function){
        _loadSpriteF=function;
    }
    void driverNewSpriteScale(_newSpriteScale function){
        _newSpriteScaleF=function;
    }
    void driverNewSpriteColor(_newSpriteColor function){
        _newSpriteColorF=function;
    }
    void driverNewSpriteRotation(_newSpriteRotation function){
        _newSpriteRotationF=function;
    }
    void driverClearSprite(_clearSprite function){
        _clearSpriteF=function;
    }
    void driverUpdateScreen(_updateScreen function){
        _updateScreenF=function;
    }
    void DrawToScreen(const std::string& original, int x, int y){
        if(_drawToScreenF)
            _drawToScreenF(original,x,y);
    }
    void LoadSprite(const std::string& filename, const std::string& label){
        if(_loadSpriteF)
            _loadSpriteF(filename,label);

    }
    void NewSpriteScale(const std::string& original, const std::string& label, int newWidth, int newHeight){
        if(_newSpriteScaleF)
            _newSpriteScaleF(original,label,newWidth,newHeight);
    }
    void NewSpriteColor(const std::string& original, const std::string& label, int red, int green, int blue, int alpha){
        if(_newSpriteColorF)
            _newSpriteColorF(original,label,red,blue,green,alpha);
    }
    void NewSpriteRotation(const std::string& original, const std::string& label, double angle){
        if(_newSpriteRotationF)
            _newSpriteRotationF(original,label,angle);
    }
    void ClearSprite(const std::string& label){
        if(_clearSpriteF)
            _clearSpriteF(label);
    }
    void UpdateScreen(){
        if(_updateScreenF)
            _updateScreenF();
    }
}
}