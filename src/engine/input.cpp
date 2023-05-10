
#include "../engine/input.h"
#include "../engine/time.h"
#include "../logger.h"
#include "../main.h"
#include <map>

namespace engine {
namespace input {
    ControllerData controllerData[MAX_CONTROLLERS];
    
    void SetButton(int controller, Controller button, bool down){
        if(controllerData[controller].buttonMap[button]!=down){
            controllerData[controller].buttonMap[button]=down;
            if(down)
                _buttonDown(controller,button);
            else
                _buttonReleased(controller,button);
        }
    }
    void SetAxis(int controller, Controller button,int value){
        if(controllerData[controller].axisMap[button]!=value){
            controllerData[controller].axisMap[button]=value;
            _axisMoved(controller,button,value);
        }
    }

    void _buttonDown(int controller, Controller button){
        controllerData[controller].buttonUpdateMap[button]=time::getCurrentTimeMillis();
        log(INFO)<< "Button Downed: "<<(int)button<<" Controller: "<<controller;
        if(button==Controller::FUNCTION)
            KillGame();
    }
    void _buttonReleased(int controller, Controller button){
        log(INFO)<< "Button Released: "<<(int)button<<" Controller: "<<controller;
        if(time::getCurrentTimeMillis()-controllerData[controller].buttonUpdateMap[button]<300)
            _buttonTapped(controller, button);
    }
    void _buttonTapped(int controller, Controller button){
        log(INFO)<< "Button Tapped: "<<(int)button<<" Controller: "<<controller;
    }
    void _axisMoved(int controller, Controller button, int value){
        log(INFO)<< "Axis Moved: "<<(int)button<<" Value: "<<value<<" Controller: "<<controller;
    }

}

}