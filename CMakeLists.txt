# Specify the minimum required version of CMake
cmake_minimum_required(VERSION 3.5)

# Set the project name and game name
set(PROJECT_NAME "example_game")
set(GAME_NAME "butcherblock")

# Specify the C++ standard
#set(CMAKE_CXX_STANDARD 11)

# Define an option to enable/disable debugging
option(DEBUG "Build with debugging information" OFF)

# Define the graphics library to use
set(GRAPHICS_LIBRARY "SDL1.2" CACHE STRING "The graphics library to use (SDL1.2, SDL2, or SFML)")

# Ensure that only one graphics library is used
if(GRAPHICS_LIBRARY STREQUAL "SDL1.2")
    set(USE_SDL1 ON)
elseif(GRAPHICS_LIBRARY STREQUAL "SDL2")
    set(USE_SDL2 ON)
else()
    message(FATAL_ERROR "Invalid graphics library specified: ${GRAPHICS_LIBRARY}")
endif()

# Set the default build type to Release
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Set the output directory for binaries and libraries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Find SDL1.2
if(USE_SDL1)
    find_package(SDL REQUIRED)
    set(SDL_INCLUDE_DIR ${SDL_INCLUDE_DIR})
    set(SDL_LIBRARY ${SDL_LIBRARY})
endif()

# Check if SDL2 is requested
if(USE_SDL2)
    find_package(SDL2 REQUIRED)
    set(SDL_INCLUDE_DIR ${SDL2_INCLUDE_DIRS})
    set(SDL_LIBRARY ${SDL2_LIBRARIES})
endif()

# Find all source files in the src directory
file(GLOB SOURCES src/*.cpp)

# Create the executable target
add_executable(${GAME_NAME} ${SOURCES})

# Link with the selected graphics library
target_link_libraries(${GAME_NAME} ${SDL_LIBRARY})

# Set include directories
target_include_directories(${GAME_NAME} PRIVATE ${SDL_INCLUDE_DIR})

# Set the compilation flags
if(DEBUG)
    target_compile_definitions(${GAME_NAME} PRIVATE -DDEBUG)
    target_compile_options(${GAME_NAME} PRIVATE -g)
else()
    target_compile_definitions(${GAME_NAME} PRIVATE -DNDEBUG)
    target_compile_options(${GAME_NAME} PRIVATE -O3)
endif()
