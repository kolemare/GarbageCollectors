#!/bin/bash

cd application
cd src

file="GCVisualizer.hpp"
define="WINDOWS"

# Create a backup of GCVisualizer.hpp
cp "$file" temp.hpp

found=false
while IFS= read -r line; do
    if [ "$line" == "#define $define" ]; then
        found=true
    else
        echo "$line"
    fi
done < "$file" > temp.txt

if ! $found; then
    echo "#define $define" > temp2.txt
    cat temp2.txt temp.txt > "$file"
    rm temp2.txt
else
    mv temp.txt "$file"
fi

cd ../..

BUILD_DIR="build"
BIN_DIR="bin"
CLEAN_FLAG=0

cd application

if [ "$1" == "--clean" ]; then
  CLEAN_FLAG=1
fi

if [ $CLEAN_FLAG -eq 1 ]; then
  # Cleaning now...
  if [ -d "$BIN_DIR" ]; then
    rm -rf "$BIN_DIR"
  fi

  cd ..

  if [ -d "$BUILD_DIR" ]; then
    rm -rf "$BUILD_DIR"
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

cd ..

cd application
cd src

# Revert the changes to GCVisualizer.hpp
cp -f temp.hpp "$file"
rm temp.hpp

cd ../..

# Navigate to the application directory
cd application

# Copy the GCDisplay.py file from the src directory to the bin directory
cp src/GCDisplay.py bin/

# Navigate back to the root directory
cd ..