#!/bin/bash

if [ -z "$BASH" ]; then
  echo "Error: This script must be run with bash."
  echo "Example: bash build-garlicos.sh --clean -j 12"
  exit 1
fi

set -e

# Set default values
GAME_PROPER_NAME="Example Game"
BUILD_DIR="build-rg35xx-garlic"
WORKSPACE=$(dirname $(dirname $(pwd)))
ZIP_NAME=$(basename "$WORKSPACE")"-rg35xx-garlic.zip"
CXXFLAGS="-mcpu=cortex-a9 -mtune=cortex-a9 -march=armv7-a -marm -mfpu=neon -mfloat-abi=softfp"
PACKAGESPACE=$(pwd)

# Parse command-line arguments
while [[ $# -gt 0 ]]; do
    key="$1"

    case $key in
        -j)
        CORES="$2"
        shift # past argument
        shift # past value
        ;;
        --clean)
        CLEAN="true"
        shift # past argument
        ;;
        --debug)
        DEBUG="true"
        shift # past argument
        ;;
        *)    # unknown option
        echo "Unknown option: $1"
        echo "Options:"
        echo "-j # | run make with designated number of cores, default: 1"
        echo "--clean | wipes build directory before running"
        echo "--debug | enables debug logging and options"
        exit 1
        ;;
    esac
done

# Check if the cores value is a numerical value
if [[ ! $CORES =~ ^[0-9]+$ ]] && [ ! -z "$CORES" ]; then
    echo "The value passed with -j must be a positive integer"
    exit 1
fi

# Travel to workspace
cd ../..

# Handle clean option
if [ "$CLEAN" == "true" ]; then
    if [ -d "$WORKSPACE/$BUILD_DIR" ]; then
        rm -rf "$WORKSPACE/$BUILD_DIR"
    fi
fi

# Create build directory if it doesn't exist
if [ ! -d "$WORKSPACE/$BUILD_DIR" ]; then
    mkdir "$WORKSPACE/$BUILD_DIR"
fi

# Run cmake with CXXFLAGS appended to the command line
cd "$WORKSPACE/$BUILD_DIR" || exit
if [ "$DEBUG" == "true" ]; then
    DEBUG_APPEND="-DDEBUG=ON"
fi
cmake .. -DCMAKE_CXX_FLAGS="$CXXFLAGS" $DEBUG_APPEND

# Run make with the specified number of cores (if any)
if [ ! -z "$CORES" ]; then
    make -j "$CORES"
else
    make
fi

# Prepare package environment
cp -R $PACKAGESPACE/package .
mkdir package/ROMS/PORTS/$(basename "$WORKSPACE")
cd bin && cp * ../package/ROMS/PORTS/$(basename "$WORKSPACE") && cd ..
cd .. && cp -R assets $BUILD_DIR/package/ROMS/PORTS/$(basename "$WORKSPACE") && cd $BUILD_DIR
cd package
# Modify launch script and icon
sed -i "s/GAME_NAME/$(basename "$WORKSPACE")/g" ./ROMS/PORTS/GAME_NAME.sh
mv ROMS/PORTS/GAME_NAME.sh "ROMS/PORTS/$GAME_PROPER_NAME.sh"
mv ROMS/PORTS/Imgs/GAME_NAME.png "ROMS/PORTS/Imgs/$GAME_PROPER_NAME.png"

# Generate zip
zip -r ../$ZIP_NAME *
