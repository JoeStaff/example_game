
#include "input.h"
#include "../engine/time.h"
#include "../logger.h"
#include "../main.h"
#include <map>

namespace engine {
namespace input {
    ControllerData controllerData[MAX_CONTROLLERS];
    std::map<const std::string,EventFunction> eventFunctions;
    std::map<const std::string,bool> eventFunctionsClear;
    
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

    void addEventFunction(const std::string& label, EventFunction function){
        eventFunctions[label]=function;
    }
    void removeEventFunction(const std::string& label){
        if (eventFunctions.find(label) != eventFunctions.end())
            eventFunctionsClear[label]=true;
    }
    void removeAllEventFunctions(){
        for (const auto& func : eventFunctions)
            eventFunctionsClear[func.first]=true;
    }

    void setButtonFunction(int controller, Controller button, ButtonActionType buttonActionType, ButtonFunction function){
        switch(buttonActionType){
            case ButtonActionType::Up:
                if(controller==-1)
                    for(int i=0; i<MAX_CONTROLLERS; i++)
                        controllerData[i].upFunctionMap[button]=function;
                if(controller>=0 && controller<MAX_CONTROLLERS)
                    controllerData[controller].upFunctionMap[button]=function;
                break;
            case ButtonActionType::Down:
                if(controller==-1)
                    for(int i=0; i<MAX_CONTROLLERS; i++)
                        controllerData[i].downFunctionMap[button]=function;
                if(controller>=0 && controller<MAX_CONTROLLERS)
                    controllerData[controller].downFunctionMap[button]=function;
                break;
            case ButtonActionType::Tap:
                if(controller==-1)
                    for(int i=0; i<MAX_CONTROLLERS; i++)
                        controllerData[i].tapFunctionMap[button]=function;
                if(controller>=0 && controller<MAX_CONTROLLERS)
                    controllerData[controller].tapFunctionMap[button]=function;
                break;
        }
    }
    void clearButtonFunction(int controller, Controller button, ButtonActionType buttonActionType){
        switch(buttonActionType){
            case ButtonActionType::Down:
                if(controller==-1)
                    for(int i=0; i<MAX_CONTROLLERS; i++)
                        if (controllerData[i].downFunctionMap.find(button) != controllerData[i].downFunctionMap.end())
                            controllerData[i].downFunctionMap.erase(button);
                if ((controller>=0 && controller<MAX_CONTROLLERS) && controllerData[controller].downFunctionMap.find(button) != controllerData[controller].downFunctionMap.end())
                    controllerData[controller].downFunctionMap.erase(button);
                break;
            case ButtonActionType::Up:
                if(controller==-1)
                    for(int i=0; i<MAX_CONTROLLERS; i++)
                        if (controllerData[i].upFunctionMap.find(button) != controllerData[i].upFunctionMap.end())
                            controllerData[i].upFunctionMap.erase(button);
                if ((controller>=0 && controller<MAX_CONTROLLERS) && controllerData[controller].upFunctionMap.find(button) != controllerData[controller].upFunctionMap.end())
                    controllerData[controller].upFunctionMap.erase(button);
                break;
            case ButtonActionType::Tap:
                if(controller==-1)
                    for(int i=0; i<MAX_CONTROLLERS; i++)
                        if (controllerData[i].tapFunctionMap.find(button) != controllerData[i].tapFunctionMap.end())
                            controllerData[i].tapFunctionMap.erase(button);
                if ((controller>=0 && controller<MAX_CONTROLLERS) && controllerData[controller].tapFunctionMap.find(button) != controllerData[controller].tapFunctionMap.end())
                    controllerData[controller].tapFunctionMap.erase(button);
                break;
        }
    }
    void clearAllButtonFunction(int controller){
        if(controller==-1){
            for(int i=0; i<MAX_CONTROLLERS; i++){
                controllerData[i].downFunctionMap.clear();
                controllerData[i].upFunctionMap.clear();
                controllerData[i].tapFunctionMap.clear();
            }
        }else if(controller>=0 && controller<MAX_CONTROLLERS){
            controllerData[controller].downFunctionMap.clear();
            controllerData[controller].upFunctionMap.clear();
            controllerData[controller].tapFunctionMap.clear();
        }
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
        for (const auto& func : eventFunctions)
            func.second(controller, button, ButtonActionType::Down);
        for (const auto& func : eventFunctionsClear)
            if(func.second){
                eventFunctions.erase(func.first);
                eventFunctionsClear[func.first]=false;
            }
    }
    void _buttonReleased(int controller, Controller button){
        log(INFO)<< "Button Released: "<<(int)button<<" Controller: "<<controller;
        if (controllerData[controller].upFunctionMap.find(button) != controllerData[controller].upFunctionMap.end())
        {
            const auto& buttonFunction=controllerData[controller].upFunctionMap[button];
            buttonFunction(controller);
        }
        for (const auto& func : eventFunctions)
            func.second(controller, button, ButtonActionType::Up);
        for (const auto& func : eventFunctionsClear)
            if(func.second){
                eventFunctions.erase(func.first);
                eventFunctionsClear[func.first]=false;
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
        for (const auto& func : eventFunctions)
            func.second(controller, button, ButtonActionType::Tap);
        for (const auto& func : eventFunctionsClear)
            if(func.second){
                eventFunctions.erase(func.first);
                eventFunctionsClear[func.first]=false;
            }
    }
    void _axisMoved(int controller, Controller button, int value){
        log(INFO)<< "Axis Moved: "<<(int)button<<" Value: "<<value<<" Controller: "<<controller;
        for (const auto& func : eventFunctions)
            func.second(controller, button, ButtonActionType::Axis);
        for (const auto& func : eventFunctionsClear)
            if(func.second){
                eventFunctions.erase(func.first);
                eventFunctionsClear[func.first]=false;
            }
    }

    std::string GetButtonName(Controller button,const std::string& controllerName){
        std::string result="Unknown Button";
        switch(button){
            case Controller::SOUTH:
                result="South";
                break;
            case Controller::EAST:
                result="East";
                break;
            case Controller::WEST:
                result="West";
                break;
            case Controller::NORTH:
                result="North";
                break;
            case Controller::DPAD_UP:
                result="Dpad Up";
                break;
            case Controller::DPAD_RIGHT:
                result="Dpad Right";
                break;
            case Controller::DPAD_DOWN:
                result="Dpad Down";
                break;
            case Controller::DPAD_LEFT:
                result="Dpad Left";
                break;
            case Controller::LEFT_BUMPER:
                if(controllerName.compare("RG35XX Gamepad")==0)
                    result="L1";
                else
                    result="Left Bumper";
                break;
            case Controller::LEFT_TRIGGER:
                if(controllerName.compare("RG35XX Gamepad")==0)
                    result="L2";
                else
                    result="Left Trigger";
                break;
            case Controller::RIGHT_BUMPER:
                if(controllerName.compare("RG35XX Gamepad")==0)
                    result="R1";
                else
                    result="Right Bumper";
                break;
            case Controller::RIGHT_TRIGGER:
                if(controllerName.compare("RG35XX Gamepad")==0)
                    result="R2";
                else
                    result="Right Trigger";
                break;
            case Controller::LEFT_STICK:
                result="Left Stick";
                break;
            case Controller::RIGHT_STICK:
                result="Right Stick";
                break;
            case Controller::START:
                result="Start";
                break;
            case Controller::SELECT:
                result="Select";
                break;
            case Controller::POWER:
                result="Power";
                break;
            case Controller::FUNCTION:
                if(controllerName.compare("RG35XX Gamepad")==0)
                    result="Menu";
                else
                    result="Function";
                break;
            case Controller::VOLUME_UP:
                result="Volume Up";
                break;
            case Controller::VOLUME_DOWN:
                result="Volume Down";
                break;
            case Controller::LEFT_STICK_H:
                result="Left Analog Stick X";
                break;
            case Controller::LEFT_STICK_V:
                result="Left Analog Stick Y";
                break;
            case Controller::RIGHT_STICK_H:
                result="Right Analog Stick X";
                break;
            case Controller::RIGHT_STICK_V:
                result="Right Analog Stick Y";
                break;
        }
        return result;
    }

}

}