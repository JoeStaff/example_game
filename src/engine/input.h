#ifndef ENGINE_INPUT_H
#define ENGINE_INPUT_H

#include <map>
#include <functional>

namespace engine{
namespace input{

    constexpr int MAX_CONTROLLERS=16;
    
    typedef std::function<void*(int)> ButtonFunction;

    enum class ButtonActionType{
        Up,
        Down,
        Tap
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
    void setButtonFunction(int controller, Controller button, ButtonActionType buttonActionType, ButtonFunction function);
    void clearButtonFunction(int controller, Controller button, ButtonActionType buttonActionType);
    void clearAllButtonFunction(int controller);
    void _buttonDown(int controller, Controller button);
    void _buttonReleased(int controller, Controller button);
    void _buttonTapped(int controller, Controller button);
    void _axisMoved(int controller, Controller button, int value);

}

}
#endif