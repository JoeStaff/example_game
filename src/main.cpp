#include "logger.h"
#include "sdl1/init.h"
#include "engine/image.h"
#include "engine/input.h"
#include "engine/audio.h"
#include <chrono>
#include <thread>

bool gameRunning;
int size=32;

void* fart(int controller){
    if(controller==0)
        engine::audio::playSoundEffect("fart",128,0);
}

void* rescale(int controller){
    switch(size){
        case 32:
            size=64;
            break;
        case 64:
            size=128;
            break;
        case 128:
            size=16;
            break;
        case 16:
            size=32;
            break;
    }
    engine::image::LoadSprite("test.png","original");
    engine::image::NewSpriteScale("original","original",size,size);
}

int main(int argc, char* argv[]) {
    log(INFO)<<"Initilizations Begin";
    gameRunning=true;
    sdl1::init::Initiate();
    engine::input::setButtonFunction(0,engine::input::Controller::SOUTH,engine::input::ButtonActionType::Tap,&fart);
    engine::input::setButtonFunction(0,engine::input::Controller::NORTH,engine::input::ButtonActionType::Tap,&rescale);
    engine::audio::loadSoundEffect("assets/sounds/fart.ogg","fart");
    engine::image::LoadSprite("test.png","original");
    engine::image::NewSpriteScale("original","original",size,size);
    double rot=0;
    int tick=0;

    std::chrono::milliseconds tickDelay(16);  // Desired delay between loops
    auto tickStart = std::chrono::steady_clock::now();  // Start time of the loop
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    while (gameRunning) {
        auto startTime = std::chrono::steady_clock::now();  // Start time of the loop

        tick++;
        if(tick%60==0){
            auto currentTime=std::chrono::steady_clock::now();
            double difference=std::chrono::duration_cast<std::chrono::duration<double>>(currentTime-tickStart).count();
            log(INFO)<<"FPS: "<<(60/(difference));
            tickStart=currentTime;
        }

        rot+=0.01;
        engine::image::NewSpriteRotation("original","clone",rot);
        int x,y,r,g,b;
        for (int i = 0; i < 256; ++i) {
            x = std::rand() % (640-(size));  // Generate a random x value between 0 and 99
            y = std::rand() % (480-(size));  // Generate a random y value between 0 and 99
            if(i%20==0){
                r = std::rand() % 128;
                g = std::rand() % 128;
                b = std::rand() % 128;
                engine::image::NewSpriteColor("clone","clone2",r+127,g+127,b+127,255);
            }
            engine::image::DrawToScreen("clone2",x,y);            
        }
        engine::image::UpdateScreen();

        auto endTime = std::chrono::steady_clock::now();  // End time of the loop
        auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime); // Calculate the actual elapsed time
        auto remainingTime = tickDelay - elapsedTime; // Calculate the remaining time to achieve the desired delay
        if (remainingTime > std::chrono::milliseconds(0)) {
            std::this_thread::sleep_for(remainingTime);  // Wait for the remaining time
        }
    }
    sdl1::init::Quit();
    return 0;
}
bool IsGameRunning(){
    return gameRunning;
}
bool KillGame(){
    gameRunning=false;
}