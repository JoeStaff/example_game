#ifndef ENGINE_INPUT_H
#define ENGINE_INPUT_H

#include <map>

namespace engine{
namespace input{

    constexpr int MAX_CONTROLLERS=16;

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

    struct ControllerData
    {
        ButtonMap buttonMap;
        ButtonUpdateMap buttonUpdateMap;
        AxisMap axisMap;
    };
    void SetButton(int controller, Controller button, bool down);
    void SetAxis(int controller, Controller button,int value);
    void _buttonDown(int controller, Controller button);
    void _buttonReleased(int controller, Controller button);
    void _buttonTapped(int controller, Controller button);
    void _axisMoved(int controller, Controller button, int value);

}

}
#endif