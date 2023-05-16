#include "init.h"
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "input.h"
#include "audio.h"
#include "image.h"
#include "../logger.h"
#include "../main.h"

namespace sdl1{
namespace init{
    void Initiate(){
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            log(ERROR)<<(SDL_GetError());
            return;
        }
        log(INFO) << "SDL initialized successfully.";

        //Debug Tests::To be deleted
        sdl1::input::Initiate();
        sdl1::audio::Initiate();
        sdl1::image::Initiate();
    }
}
}