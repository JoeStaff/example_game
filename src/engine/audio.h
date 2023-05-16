#ifndef ENGINE_AUDIO_H
#define ENGINE_AUDIO_H
#include <string>
namespace engine{
namespace audio{

    typedef bool (*_loadSoundEffect)(const std::string&, const std::string&);
    typedef void (*_unloadSoundEffect)(const std::string&);
    typedef void (*_playSoundEffect)(const std::string&, int, int);
    typedef void (*_pauseSoundEffects)();
    typedef void (*_resumeSoundEffects)();
    void driverLoadSoundEffect(_loadSoundEffect function);
    void driverunloadSoundEffect(_unloadSoundEffect function);
    void driverplaySoundEffect(_playSoundEffect function);
    void driverpauseSoundEffects(_pauseSoundEffects function);
    void driverresumeSoundEffects(_resumeSoundEffects function);   
    bool loadSoundEffect(const std::string& filename, const std::string& name);
    bool unloadSoundEffect(const std::string& name);
    bool playSoundEffect(const std::string& name, int volume, int loops);
    bool pauseSoundEffects();
    bool resumeSoundEffects();
}
}
#endif