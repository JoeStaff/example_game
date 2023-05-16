#ifndef SDL_AUDIO_H
#define SDL_AUDIO_H

#include <string>
#include <SDL/SDL_mixer.h>

namespace sdl1{
namespace audio{

    void Initiate();
    bool loadSoundEffect(const std::string& filename, const std::string& name);
    void unloadSoundEffect(const std::string& name);
    void playSoundEffect(const std::string&, int, int);
    void pauseSoundEffects();
    void resumeSoundEffects();
    void quitSDL();
    
}
}
#endif