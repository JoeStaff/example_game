#include "logger.h"
#ifdef USE_SDL1
    #include "sdl1/init.h"
#endif
#include "engine/image.h"
#include "engine/input.h"
#include "engine/audio.h"
#include <chrono>
#include <thread>

bool gameRunning; //True/False if game is running, setting to false initates exit
int tick; //simple value representing amount of 'ticks' (or game process cycles) since start

int main(int argc, char* argv[]) {
    log(INFO)<<"Initilizations Begin";
    gameRunning=true;

    //Initiate Drivers
    #ifdef USE_SDL1
        sdl1::init::Initiate();
    #endif

    //Fairly standard set up for game loop
    std::chrono::milliseconds tickDelay(16);  // Desired delay between loops
    auto tickStart = std::chrono::steady_clock::now();  // Start time of the loop
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    log(INFO)<<"Game Process Cycle Begins";
    while (gameRunning) {
        auto startTime = std::chrono::steady_clock::now();  // Start time of the loop

        tick++;
        #ifdef DEBUG
            if(tick%60==0){
                auto currentTime=std::chrono::steady_clock::now();
                double difference=std::chrono::duration_cast<std::chrono::duration<double>>(currentTime-tickStart).count();
                log(INFO)<<"FPS: "<<(60/(difference));
                tickStart=currentTime;
            }
        #endif

        //code goes here

        auto endTime = std::chrono::steady_clock::now();  // End time of the loop
        auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime); // Calculate the actual elapsed time
        auto remainingTime = tickDelay - elapsedTime; // Calculate the remaining time to achieve the desired delay
        if (remainingTime > std::chrono::milliseconds(0)) {
            std::this_thread::sleep_for(remainingTime);  // Wait for the remaining time
        }
    }
    log(INFO)<<"Game Process Cycle Ends, closing game";

    //Outro for Drivers
    #ifdef USE_SDL1
        sdl1::init::Quit();
    #endif

    return 0;
}
bool IsGameRunning(){
    return gameRunning;
}
bool KillGame(){
    gameRunning=false;
}
int GetTick(){
    return tick;
}