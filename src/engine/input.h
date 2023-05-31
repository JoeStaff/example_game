#ifndef ENGINE_INPUT_H
#define ENGINE_INPUT_H

#include <map>
#include <functional>

namespace engine{
namespace input{

    constexpr int MAX_CONTROLLERS=16;
    

    enum class ButtonActionType{
        Up,
        Down,
        Tap,
        Axis
    };

    enum class Controller{
        SOUTH,
        EAST,
        WEST,
        NORTH,
        DPAD_UP,
        DPAD_RIGHT,
        DPAD_DOWN,
        DPAD_LEFT,
        LEFT_BUMPER,
        LEFT_TRIGGER,
        RIGHT_BUMPER,
        RIGHT_TRIGGER,
        LEFT_STICK,
        RIGHT_STICK,
        START,
        SELECT,
        POWER,
        FUNCTION,
        VOLUME_UP,
        VOLUME_DOWN,
        LEFT_STICK_H,
        LEFT_STICK_V,
        RIGHT_STICK_H,
        RIGHT_STICK_V
    };

    typedef std::function<void*(int)> ButtonFunction;
    typedef std::function<void*(int,Controller,ButtonActionType)> EventFunction;
    typedef std::string (*_getControllerName)(int);
    typedef std::string (*_getRawInputAsString)(int,Controller);
    using ButtonMap = std::map<Controller, bool>;
    using ButtonUpdateMap = std::map<Controller, long long>;
    using AxisMap = std::map<Controller, int>;
    using ButtonFunctionMap = std::map<Controller, ButtonFunction>;
    

    struct ControllerData
    {
        ButtonMap buttonMap;
        ButtonUpdateMap buttonUpdateMap;
        AxisMap axisMap;
        ButtonFunctionMap downFunctionMap;
        ButtonFunctionMap upFunctionMap;
        ButtonFunctionMap tapFunctionMap;
    };
    void SetButton(int controller, Controller button, bool down);
    void SetAxis(int controller, Controller button,int value);
    void addEventFunction(const std::string& label, EventFunction function);
    void removeEventFunction(const std::string& label);
    void removeAllEventFunctions();
    void setButtonFunction(int controller, Controller button, ButtonActionType buttonActionType, ButtonFunction function);
    void clearButtonFunction(int controller, Controller button, ButtonActionType buttonActionType);
    void clearAllButtonFunction(int controller);
    bool IsButtonPressed(int controller, Controller button);
    long long GetButtonPressTime(int controller, Controller button);
    int GetAxisValue(int controller, Controller button);
    void _buttonDown(int controller, Controller button);
    void _buttonReleased(int controller, Controller button);
    void _buttonTapped(int controller, Controller button);
    void _axisMoved(int controller, Controller button, int value);

    void driverGetControllerName(_getControllerName function);
    std::string GetControllerName(int controller);
    void driverGetRawInputAsString(_getRawInputAsString function);
    std::string GetRawInputAsString(int controller, Controller button);
    std::string GetButtonName(Controller button,const std::string& controllerName);

}

}
#endif