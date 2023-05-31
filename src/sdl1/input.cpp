//#ifdef USE_SDL1

#include <SDL/SDL.h>
#include <SDL/SDL_thread.h>
#include <fstream>
#include <sstream>
#include <limits>
#include <vector>
#include <map>
#include <unordered_set>
#include "../logger.h"
#include "../engine/input.h"
#include "../engine/file.h"
#include "input.h"
#include "audio.h"
#include "../main.h"

namespace sdl1 {
namespace input {

    SDL_Thread* thread = NULL;
    JoystickData joystickData[engine::input::MAX_CONTROLLERS];

    void Initiate(){
		if (SDL_InitSubSystem(SDL_INIT_JOYSTICK) < 0){
			log(ERROR) << SDL_GetError();
			log(WARNING) << "Error initializing Joysticks.";
            return;
		}
        log(INFO) << "SDL input initialized successfully.";
		SDL_JoystickEventState(SDL_ENABLE);
        ConnectControllers();
        int threadID;
        thread=SDL_CreateThread(PollEvents,&threadID);

        engine::input::driverGetControllerName(&GetControllerName);
        engine::input::driverGetRawInputAsString(&GetRawInputAsString);

    }

    void ConnectControllers(){
        int numJoysticks = SDL_NumJoysticks();
        if (numJoysticks == 0)
        {
            log(INFO) << "No joysticks available";
            return;
        }
        int numConnectedJoysticks = 0;
        for (int i = 0; i < numJoysticks && numConnectedJoysticks < engine::input::MAX_CONTROLLERS; i++)
        {
            SDL_Joystick* joystick = SDL_JoystickOpen(i);
            if (joystick == NULL)
            {
                log(ERROR) << "Failed to open joystick " << i;
                continue;
            }
            const std::string joystickName = SDL_JoystickName(SDL_JoystickIndex(joystick));
            log(INFO) << "Joystick " << i << " Name: " << joystickName;

            
            const std::string filename = "joystick_mappings.txt";
            
            std::vector<FileButtonMapping> mappings;
            mappings = findButtonMappings(filename, joystickName);
            
            if (mappings.empty()) {
                log(INFO) << "JoystickName not found or no button mappings exist for that joystick. Using Generic";
                const std::string joystickName="Generic";
                mappings = findButtonMappings(filename, joystickName);
                if (mappings.empty()){
                    log(ERROR) << "No Generic input found";
                }
            }
            
                log(INFO) << "Button mappings for " << joystickName << " loaded.";
                for (const auto& mapping : mappings) {
                    log(INFO) << "Type: " << mapping.type << ", InputID: " << mapping.inputID
                            << ", Label: " << mapping.label << ", HatID: " << mapping.hatID;
                }

            MapController(i,mappings);
            numConnectedJoysticks++;
        }
    }

    int PollEvents(void* data){
        SDL_Event e;
        while(IsGameRunning()){

            while(SDL_PollEvent(&e)){
                switch(e.type){
                    case SDL_JOYBUTTONDOWN:
                        engine::input::SetButton(e.jbutton.which,joystickData[e.jbutton.which].buttonMap[e.jbutton.button],true);
                        break;
                    case SDL_JOYBUTTONUP:
                        engine::input::SetButton(e.jbutton.which,joystickData[e.jbutton.which].buttonMap[e.jbutton.button],false);
                        break;
                    case SDL_JOYHATMOTION:
                        for( const auto& hat : joystickData[e.jhat.which].hatMap){
                            if(e.jhat.value&hat.first)
                                engine::input::SetButton(e.jhat.which,hat.second,true);
                            else
                                engine::input::SetButton(e.jhat.which,hat.second,false);
                        }
                        break;
                    case SDL_JOYAXISMOTION:
                        engine::input::SetAxis(e.jaxis.which,joystickData[e.jaxis.which].axisMap[e.jaxis.axis],e.jaxis.value);
                        break;
                }
            }
            SDL_Delay(10);
        }
        return 0;
    }

    void MapController(int controller, std::vector<FileButtonMapping> mappings){
        std::string ButtonLabel;
        std::string InputType;
        engine::input::Controller buttonid;
        for (const auto& mapping : mappings) {
            ButtonLabel=mapping.label;
            InputType=mapping.type;
            if (ButtonLabel == "SOUTH") {
                buttonid=engine::input::Controller::SOUTH;
            } else if (ButtonLabel == "EAST") {
                buttonid=engine::input::Controller::EAST;
            } else if (ButtonLabel == "WEST") {
                buttonid=engine::input::Controller::WEST;
            } else if (ButtonLabel == "NORTH") {
                buttonid=engine::input::Controller::NORTH;
            } else if (ButtonLabel == "DPAD_UP") {
                buttonid=engine::input::Controller::DPAD_UP;
            } else if (ButtonLabel == "DPAD_RIGHT") {
                buttonid=engine::input::Controller::DPAD_RIGHT;
            } else if (ButtonLabel == "DPAD_DOWN") {
                buttonid=engine::input::Controller::DPAD_DOWN;
            } else if (ButtonLabel == "DPAD_LEFT") {
                buttonid=engine::input::Controller::DPAD_LEFT;
            } else if (ButtonLabel == "LEFT_BUMPER") {
                buttonid=engine::input::Controller::LEFT_BUMPER;
            } else if (ButtonLabel == "LEFT_TRIGGER") {
                buttonid=engine::input::Controller::LEFT_TRIGGER;
            } else if (ButtonLabel == "RIGHT_BUMPER") {
                buttonid=engine::input::Controller::RIGHT_BUMPER;
            } else if (ButtonLabel == "RIGHT_TRIGGER") {
                buttonid=engine::input::Controller::RIGHT_TRIGGER;
            } else if (ButtonLabel == "LEFT_STICK") {
                buttonid=engine::input::Controller::LEFT_STICK;
            } else if (ButtonLabel == "RIGHT_STICK") {
                buttonid=engine::input::Controller::RIGHT_STICK;
            } else if (ButtonLabel == "START") {
                buttonid=engine::input::Controller::START;
            } else if (ButtonLabel == "SELECT") {
                buttonid=engine::input::Controller::SELECT;
            } else if (ButtonLabel == "POWER") {
                buttonid=engine::input::Controller::POWER;
            } else if (ButtonLabel == "FUNCTION") {
                buttonid=engine::input::Controller::FUNCTION;
            } else if (ButtonLabel == "VOLUME_UP") {
                buttonid=engine::input::Controller::VOLUME_UP;
            } else if (ButtonLabel == "VOLUME_DOWN") {
                buttonid=engine::input::Controller::VOLUME_DOWN;
            } else if (ButtonLabel == "LEFT_STICK_H") {
                buttonid=engine::input::Controller::LEFT_STICK_H;
            } else if (ButtonLabel == "LEFT_STICK_V") {
                buttonid=engine::input::Controller::LEFT_STICK_V;
            } else if (ButtonLabel == "RIGHT_STICK_H") {
                buttonid=engine::input::Controller::RIGHT_STICK_H;
            } else if (ButtonLabel == "RIGHT_STICK_V") {
                buttonid=engine::input::Controller::RIGHT_STICK_V;
            } else {
                // Response for no match
            }
            
            if (InputType == "Button") {
                joystickData[controller].buttonMap[mapping.inputID]=buttonid;
            } else if (InputType == "Axis") {
                joystickData[controller].axisMap[mapping.inputID]=buttonid;
            } else if (InputType == "Hat") {
                joystickData[controller].hatMap[mapping.inputID]=buttonid;
            } else {
                // Response for no match
            }
        }
    }

    std::vector<FileButtonMapping> parseButtonMappings(const std::string& line) {
        std::vector<FileButtonMapping> mappings;
        std::istringstream iss(line);
        std::string joystickName;
        std::getline(iss, joystickName, ',');
        
        log(INFO)<<"iss >> "<<joystickName;
        if (joystickName.empty())
            return mappings;  // Empty mappings if no JoystickName is found
        
        std::string token;
        while (std::getline(iss, token, ',')) {
            std::istringstream tokenIss(token);
            std::string type;
            int inputID;
            std::string label;
            int hatID = -1;  // Default value for hatID if it's not present
            
            std::getline(tokenIss, type, ':');
            if (type != "Button" && type != "Axis" && type.substr(0, 3) != "Hat")
                continue;  // Skip invalid mappings
            
            std::string inputIDStr;
            std::getline(tokenIss, inputIDStr, ':');
            inputID = std::stoi(inputIDStr);
            
            std::getline(tokenIss, label, ':');
            
            if (type.substr(0, 3) == "Hat") {
                std::string hatIDStr = type.substr(3);
                hatID = std::stoi(hatIDStr);
                type = "Hat";
            }
            
            mappings.push_back({type, inputID, label, hatID});
        }
        
        // Clear the state of iss and ignore remaining characters
        iss.clear();
        iss.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        return mappings;
    }

    std::string GetControllerName(int controller){
        return SDL_JoystickName(controller);
    }

    std::string GetRawInputAsString(int controller, engine::input::Controller button){
        for (auto inputmap : joystickData[controller].buttonMap){
            if(inputmap.second==button){
                return "Button:"+std::to_string(inputmap.first);
            }
        }
        for (auto inputmap : joystickData[controller].axisMap){
            if(inputmap.second==button){
                return "Axis:"+std::to_string(inputmap.first);
            }
        }
        for (auto inputmap : joystickData[controller].hatMap){
            if(inputmap.second==button){
                return "Hat:"+std::to_string(inputmap.first);
            }
        }
    }

    std::vector<FileButtonMapping> findButtonMappings(const std::string& filename, const std::string& joystickName) {
        std::ifstream file(filename);
        std::string line;
        
        while (std::getline(file, line)) {
            log(INFO)<<joystickName<<"|"<<line;
            log(INFO)<<"line.substr(0,"<<joystickName.size()<<"()) == "<<joystickName;
            if (line.substr(0, joystickName.size()) == joystickName) {
                return parseButtonMappings(line);
            }
        }
        
        return {};  // Empty mappings if JoystickName is not found
    }


}

}
