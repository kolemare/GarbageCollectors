#!/bin/bash

BUILD_DIR="build"
BIN_DIR="bin"

# Cleaning now...
cd application

if [ -d "$BIN_DIR" ]; then
  rm -rf "$BIN_DIR"
fi

cd ..

if [ -d "$BUILD_DIR" ]; then
  rm -rf "$BUILD_DIR"
fi
