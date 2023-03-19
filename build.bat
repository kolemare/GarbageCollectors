@echo off

set BUILD_DIR=build
set BIN_DIR=bin

cd application 

if not exist %BIN_DIR% (
  mkdir %BIN_DIR%
)
cd ..

if not exist %BUILD_DIR% (
  mkdir %BUILD_DIR%
)

cd %BUILD_DIR%

cmake ..
make