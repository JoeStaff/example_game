#ifndef SDL_INPUT_H
#define SDL_INPUT_H

#include <vector>
#include "../engine/input.h"

namespace sdl1 {
namespace input {

    struct FileButtonMapping {
        std::string type;
        int inputID;
        std::string label;
        int hatID;  // Optional field
    };
    using ButtonMap = std::map<int, engine::input::Controller>;
    using HatMap = std::map<int, engine::input::Controller>;
    using AxisMap = std::map<int, engine::input::Controller>;
    struct JoystickData
    {
        ButtonMap buttonMap;
        HatMap hatMap;
        AxisMap axisMap;
    };
    void Initiate();
    void ConnectControllers();
    int PollEvents(void* data);
    void MapController(int controller, std::vector<FileButtonMapping> mappings);
    std::vector<FileButtonMapping> findButtonMappings(const std::string& filename, const std::string& joystickName);
    std::vector<FileButtonMapping> parseButtonMappings(const std::string& line);
}
}
#endif