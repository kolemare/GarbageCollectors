@echo off

cd application
cd src

setlocal EnableDelayedExpansion

set file=GCVisualizer.hpp
set define=WINDOWS

set found=false
(for /F "tokens=*" %%a in (%file%) do (
    set line=%%a
    if "!line!"=="#define %define%" set found=true
    echo !line!
)) > temp.txt

if not %found% (
    echo #define %define%> temp2.txt
    type temp2.txt temp.txt > %file%
    del temp2.txt
) else (
    move /Y temp.txt %file%
)

cd ..
cd ..

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

REM Revert the changes to GCVisualizer.hpp
set found=false
(for /F "tokens=*" %%a in (%file%) do (
    set line=%%a
    if "!line!"=="#define %define%" set found=true
    echo !line!
)) > temp.txt

if %found% (
    (for /F "tokens=*" %%a in (%file%) do (
        set line=%%a
        if not "!line!"=="#define %define%" echo !line!
    )) > temp2.txt
    move /Y temp2.txt %file%
)

cd ..
cd ..