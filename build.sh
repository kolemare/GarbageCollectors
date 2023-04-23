#!/bin/bash

cd application/src

file="GCVisualizer.hpp"
define="WINDOWS"

# Check if #define WINDOWS exists and remove it
found=false
while read line; do
    if [ "$line" = "#define $define" ]; then
        found=true
    else
        echo "$line"
    fi
done < "$file" > temp.txt

if [ "$found" = true ]; then
    mv -f temp.txt "$file"
else
    rm temp.txt
fi

cd ../..

# Run cmake and make
BUILD_DIR="build"
BIN_DIR="bin"
CLEAN_FLAG=0

cd application

if [ "$1" = "--clean" ]; then
    CLEAN_FLAG=1
fi

if [ "$CLEAN_FLAG" = 1 ]; then
    # Cleaning now...
    if [ -d "$BIN_DIR" ]; then
        rm -r "$BIN_DIR"
    fi

    cd ..

    if [ -d "$BUILD_DIR" ]; then
        rm -r "$BUILD_DIR"
    fi

    # Building now...
    cd application
fi

if [ ! -d "$BIN_DIR" ]; then
    mkdir "$BIN_DIR"
fi

cd ..

if [ ! -d "$BUILD_DIR" ]; then
    mkdir "$BUILD_DIR"
fi

cd "$BUILD_DIR"

cmake ..
make

cd ../..

# Add #define WINDOWS back if it was removed
found=false
while read line; do
    if [ "$line" = "#define $define" ]; then
        found=true
        echo "$line"
    else
        echo "$line"
    fi
done < "$file" > temp.txt

if [ "$found" = false ]; then
    echo "#define $define" >> "$file"
fi

exit 0