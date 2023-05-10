#include "init.h"
#include <SDL/SDL.h>
#include "input.h"
#include "../logger.h"

namespace sdl1{
namespace init{
    void Initiate(){
        log(INFO) << "Test";
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            log(ERROR)<<(SDL_GetError());
            return;
        }
        log(INFO) << "SDL initialized successfully.";

        sdl1::input::Initiate();

    }
}
}