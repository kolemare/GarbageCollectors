#!/bin/bash

cd application
cd src

file="GCVisualizer.hpp"
define="WINDOWS"

file2="GCInvokeDisplaySingleton.hpp"
search_str='py GCDisplay.py &'
replace_str='python3 GCDisplay.py &'

# Create backups of GCVisualizer.hpp and GCInvokeDisplaySingleton.hpp
cp "$file" GCVisualizerTEMP.hpp
cp "$file2" GCInvokeDisplaySingletonTEMP.hpp

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

while IFS= read -r line; do
    if echo "$line" | grep -q "$search_str"; then
        echo "${line/$search_str/$replace_str}"
    else
        echo "$line"
    fi
done < "$file2" > temp2.txt

mv temp2.txt "$file2"

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

# Revert the changes to GCVisualizer.hpp and GCInvokeDisplaySingleton.hpp
cp -f GCVisualizerTEMP.hpp "$file"
cp -f GCInvokeDisplaySingletonTEMP.hpp "$file2"
rm GCVisualizerTEMP.hpp
rm GCInvokeDisplaySingletonTEMP.hpp

cd ../..

# Navigate to the application directory
cd application

# Copy the GCDisplay.py file from the src directory to the bin directory
cp src/GCDisplay.py bin/

# Navigate back to the root directory
cd ..