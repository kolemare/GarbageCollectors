#!/bin/bash

BUILD_DIR="build"
BIN_DIR="bin"
CLEAN_FLAG=0

cd application

if [ "$1" == "--clean" ]; then
  CLEAN_FLAG=1
fi

if [ $CLEAN_FLAG == 1 ]; then
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