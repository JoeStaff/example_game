#include "logger.h"
#include "sdl1/init.h"
#include "engine/input.h"
#include "engine/audio.h"
#include <SDL.h>

bool gameRunning;

void* fart(){
    engine::audio::playSoundEffect("fart",128,0);
}

int main(int argc, char* argv[]) {
    //log(INFO)<<"Initilizations Begin";
    gameRunning=true;
    sdl1::init::Initiate();
    engine::input::setButtonFunction(0,engine::input::Controller::SOUTH,engine::input::ButtonActionType::Tap,&fart);
    engine::audio::loadSoundEffect("assets/sounds/fart.ogg","fart");
    while(gameRunning){


        SDL_Delay(16);
    }
    SDL_Quit();
    return 0;
}
bool IsGameRunning(){
    return gameRunning;
}
bool KillGame(){
    gameRunning=false;
}