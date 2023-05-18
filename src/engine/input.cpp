
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

    void setButtonFunction(int controller, Controller button, ButtonActionType buttonActionType, ButtonFunction function){
        switch(buttonActionType){
            case ButtonActionType::Up:
                controllerData[controller].upFunctionMap[button]=function;
                break;
            case ButtonActionType::Down:
                controllerData[controller].downFunctionMap[button]=function;
                break;
            case ButtonActionType::Tap:
                controllerData[controller].tapFunctionMap[button]=function;
                break;
        }
    }
    void clearButtonFunction(int controller, Controller button, ButtonActionType buttonActionType){
        switch(buttonActionType){
            case ButtonActionType::Down:
                if (controllerData[controller].downFunctionMap.find(button) != controllerData[controller].downFunctionMap.end())
                {
                    controllerData[controller].downFunctionMap.erase(button);
                }
                break;
            case ButtonActionType::Up:
                if (controllerData[controller].upFunctionMap.find(button) != controllerData[controller].upFunctionMap.end())
                {
                    controllerData[controller].upFunctionMap.erase(button);
                }
                break;
            case ButtonActionType::Tap:
                if (controllerData[controller].tapFunctionMap.find(button) != controllerData[controller].tapFunctionMap.end())
                {
                    controllerData[controller].tapFunctionMap.erase(button);
                }
                break;
        }
    }
    void clearAllButtonFunction(int controller){
        controllerData[controller].downFunctionMap.clear();
        controllerData[controller].upFunctionMap.clear();
        controllerData[controller].tapFunctionMap.clear();
    }

    bool IsButtonPressed(int controller, Controller button){
        return controllerData[controller].buttonMap[button];
    }
    long long GetButtonPressTime(int controller, Controller button){
        return controllerData[controller].buttonUpdateMap[button];
    }
    int GetAxisValue(int controller, Controller button){
        return controllerData[controller].axisMap[button];
    }

    void _buttonDown(int controller, Controller button){
        controllerData[controller].buttonUpdateMap[button]=time::getCurrentTimeMillis();
        log(INFO)<< "Button Downed: "<<(int)button<<" Controller: "<<controller;

        //For Development Speediness
        if(button==Controller::FUNCTION)
            KillGame();

        if (controllerData[controller].downFunctionMap.find(button) != controllerData[controller].downFunctionMap.end())
        {
            const auto& buttonFunction=controllerData[controller].downFunctionMap[button];
            buttonFunction(controller);
        }
    }
    void _buttonReleased(int controller, Controller button){
        log(INFO)<< "Button Released: "<<(int)button<<" Controller: "<<controller;
        if (controllerData[controller].upFunctionMap.find(button) != controllerData[controller].upFunctionMap.end())
        {
            const auto& buttonFunction=controllerData[controller].upFunctionMap[button];
            buttonFunction(controller);
        }
        if(time::getCurrentTimeMillis()-controllerData[controller].buttonUpdateMap[button]<300)
            _buttonTapped(controller, button);
    }
    void _buttonTapped(int controller, Controller button){
        log(INFO)<< "Button Tapped: "<<(int)button<<" Controller: "<<controller;
        if (controllerData[controller].tapFunctionMap.find(button) != controllerData[controller].tapFunctionMap.end())
        {
            log(INFO)<<"Function Found for button tap";
            const auto& buttonFunction=controllerData[controller].tapFunctionMap[button];
            buttonFunction(controller);
        }
    }
    void _axisMoved(int controller, Controller button, int value){
        log(INFO)<< "Axis Moved: "<<(int)button<<" Value: "<<value<<" Controller: "<<controller;
    }

}

}