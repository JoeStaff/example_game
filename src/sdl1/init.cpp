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
        log(INFO) << "Test";
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            log(ERROR)<<(SDL_GetError());
            return;
        }
        log(INFO) << "SDL initialized successfully.";

        //Debug Tests::To be deleted
        sdl1::input::Initiate();
        sdl1::audio::Initiate();
        sdl1::image::Initiate();
        double rot1=100,rot2;
        int r=175,g,b=250,a;
        bool rf,gf,bf,af;
        while(IsGameRunning()){
            rot1+=0.1;
            rot2-=0.1;
            r+=(rf)?-1:1;
            if(r>=255){
                rf=true;
            }else if(r<=100){
                rf=false;
            }
            g+=(gf)?-1:1;
            if(g>=255){
                gf=true;
            }else if(g<=100){
                gf=false;
            }
            b+=(bf)?-1:1;
            if(b>=255){
                bf=true;
            }else if(b<=100){
                bf=false;
            }
            a+=(af)?-1:1;
            if(a>=255){
                af=true;
            }else if(a<=0){
                af=false;
            }
            SDL_Surface* tempa1=image::LoadSprite("test.png");
            SDL_Surface* tempa2=image::NewSpriteColor(tempa1,r,g,b,255);
            SDL_FreeSurface(tempa1);
            tempa1=image::NewSpriteRotation(tempa2,0);
            SDL_FreeSurface(tempa2);
            tempa2=image::NewSpriteScale(tempa1,64,64);
            SDL_FreeSurface(tempa1);
            image::DrawToScreen(tempa2,300,230);
            SDL_FreeSurface(tempa2);
            tempa1=image::LoadSprite("test.png");
            tempa2=image::NewSpriteColor(tempa1,255,255,255,255);
            SDL_FreeSurface(tempa1);
            tempa1=image::NewSpriteRotation(tempa2,rot1);
            SDL_FreeSurface(tempa2);
            tempa2=image::NewSpriteScale(tempa1,64,64);
            SDL_FreeSurface(tempa1);
            image::DrawToScreen(tempa2,320,240);
            SDL_FreeSurface(tempa2);
            tempa1=image::LoadSprite("test.png");
            tempa2=image::NewSpriteColor(tempa1,255,255,255,a);
            SDL_FreeSurface(tempa1);
            tempa1=image::NewSpriteRotation(tempa2,rot2);
            SDL_FreeSurface(tempa2);
            tempa2=image::NewSpriteScale(tempa1,64,64);
            SDL_FreeSurface(tempa1);
            image::DrawToScreen(tempa2,330,250);
            SDL_FreeSurface(tempa2);
            image::UpdateScreen();
            SDL_Delay(16);
        }
    }
}
}