#ifndef SDL_AUDIO_H
#define SDL_AUDIO_H

#include <string>
#include <SDL/SDL_mixer.h>

namespace sdl1{
namespace audio{

    void Initiate();
    bool loadSoundEffect(const std::string& filename, const std::string& name);
    void unloadSoundEffect(const std::string& name);
    void playSoundEffect(const std::string& name, int volume = MIX_MAX_VOLUME, int loops = 0);
    void pauseSoundEffects();
    void resumeSoundEffects();
    void quitSDL();
    
}
}
#endif