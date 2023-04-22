@echo off

set BUILD_DIR=build
set BIN_DIR=bin
set CLEAN_FLAG=0

cd application

if "%~1" == "--clean" (
  set CLEAN_FLAG=1
)

if %CLEAN_FLAG% == 1 (
  REM Cleaning now...
  if exist %BIN_DIR% (
    rd /s /q %BIN_DIR%
  )

  cd ..

  if exist %BUILD_DIR% (
    rd /s /q %BUILD_DIR%
  )

  REM Building now...
  cd application
)

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