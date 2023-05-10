
#include <iostream>
#include <vector>
#include <map>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include "../logger.h"

namespace sdl1{
namespace audio{


    const int MUSIC_CHANNEL = 0; // Sound channel reserved for music

    std::vector<Mix_Chunk*> soundEffects; // Vector to store loaded sound effects
    std::map<std::string, Mix_Chunk*> soundEffectMap; // Map to store loaded sound effects with names

    // Function to initialize SDL and SDL_mixer
    void Initiate()
    {
        if (SDL_Init(SDL_INIT_AUDIO) < 0)
        {
            log(ERROR) << "SDL initialization failed: " << SDL_GetError();
        }

        if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1)
        {
            log(ERROR) << "SDL_mixer initialization failed: " << Mix_GetError();
        }
		Mix_AllocateChannels(16);
		// Set up UI channels
		Mix_ReserveChannels(2);
		Mix_GroupChannels(1, 2, 0);
    }

    // Function to load a sound effect from a file and store it in the vector and map
    bool loadSoundEffect(const std::string& filename, const std::string& name)
    {
        Mix_Chunk* sound = Mix_LoadWAV(filename.c_str());
        if (sound == nullptr)
        {
            log(ERROR) << "Failed to load sound effect: " << Mix_GetError();
            return false;
        }

        soundEffectMap[name] = sound;
        return true;
    }

    // Function to unload a previously loaded sound effect from the vector and map
    void unloadSoundEffect(const std::string& name)
    {
        Mix_Chunk* sound = soundEffectMap[name];
        if (sound != nullptr)
        {
            Mix_FreeChunk(sound);
            soundEffectMap.erase(name);
        }
    }

    // Function to play a loaded sound effect
    void playSoundEffect(const std::string& name, int volume = MIX_MAX_VOLUME, int loops = 0)
    {
        Mix_Chunk* sound = soundEffectMap[name];
        if (sound != nullptr)
        {
            Mix_Volume(Mix_PlayChannel(-1, sound, loops), volume);
        }
    }

    // Function to pause all sound effects
    void pauseSoundEffects()
    {
        Mix_Pause(-1);
    }

    // Function to resume all paused sound effects
    void resumeSoundEffects()
    {
        Mix_Resume(-1);
    }

    // Function to quit SDL and free the loaded sound effects
    void quitSDL()
    {
        for (Mix_Chunk* sound : soundEffects)
        {
            Mix_FreeChunk(sound);
        }

        soundEffects.clear();
        soundEffectMap.clear();
        Mix_CloseAudio();
        SDL_Quit();
    }

}
}