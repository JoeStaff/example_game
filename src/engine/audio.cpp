#include <string>
#include "audio.h"

namespace engine{
namespace audio{
    /*
    bool loadSoundEffect(const std::string& filename, const std::string& name)
    void unloadSoundEffect(const std::string& name)
    void playSoundEffect(const std::string& name, int volume = MIX_MAX_VOLUME, int loops = 0)
    void pauseSoundEffects()
    void resumeSoundEffects()
    */
    _loadSoundEffect _loadSoundEffectF=nullptr;
    _unloadSoundEffect _unloadSoundEffectF=nullptr;
    _playSoundEffect _playSoundEffectF=nullptr;
    _pauseSoundEffects _pauseSoundEffectsF=nullptr;
    _resumeSoundEffects _resumeSoundEffectsF=nullptr;
    void driverLoadSoundEffect(_loadSoundEffect function){
        _loadSoundEffectF=function;
    }
    void driverunloadSoundEffect(_unloadSoundEffect function){
        _unloadSoundEffectF=function;
    }
    void driverplaySoundEffect(_playSoundEffect function){
        _playSoundEffectF=function;
    }
    void driverpauseSoundEffects(_pauseSoundEffects function){
        _pauseSoundEffectsF=function;
    }
    void driverresumeSoundEffects(_resumeSoundEffects function){
        _resumeSoundEffectsF=function;
    }
    bool loadSoundEffect(const std::string& filename, const std::string& name){
        if(_loadSoundEffectF)
            _loadSoundEffectF(filename,name);
    }
    bool unloadSoundEffect(const std::string& name){
        if(_unloadSoundEffectF)
            _unloadSoundEffectF(name);
    }
    bool playSoundEffect(const std::string& name, int volume = 128, int loops = 0){
        if(_playSoundEffectF)
            _playSoundEffectF(name,volume,loops);
    }
    bool pauseSoundEffects(){
        if(_pauseSoundEffectsF)
            _pauseSoundEffectsF();
    }
    bool resumeSoundEffects(){
        if(_resumeSoundEffectsF)
            _resumeSoundEffectsF();
    }

}
}