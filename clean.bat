@echo off

set BUILD_DIR=build
set BIN_DIR=bin


REM cleaining now...
cd application 

if exist %BIN_DIR% (
  rd /s /q %BIN_DIR%
)

cd ..

if exist %BUILD_DIR% (
  rd /s /q %BUILD_DIR%
)
