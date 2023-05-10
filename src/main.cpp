#include "sdl1/init.h"
#include <SDL.h>

bool gameRunning;
int main(int argc, char* argv[]) {
    gameRunning=true;
    sdl1::init::Initiate();
    while(gameRunning){
        SDL_Delay(16);
    }
    return 0;
}
bool IsGameRunning(){
    return gameRunning;
}
bool KillGame(){
    gameRunning=false;
}