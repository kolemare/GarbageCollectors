@echo off

cd application
cd src

setlocal EnableDelayedExpansion

set file=GCVisualizer.hpp
set define=WINDOWS
set file2=GCInvokeDisplaySingleton.hpp
set search_str=python3 GCDisplay.py ^^^&
set replace_str=py GCDisplay.py ^^^&

copy %file% GCVisualizerTEMP.hpp
copy %file2% GCInvokeDisplaySingletonTEMP.hpp

set found=false
(for /F "tokens=*" %%a in (%file%) do (
    set line=%%a
    if "!line!"=="#define %define%" set found=true
    echo !line!
)) > temp.txt

if "!found!"=="false" (
    echo #define %define%> temp2.txt
    type temp2.txt temp.txt > %file%
    del temp2.txt
) else (
    move /Y temp.txt %file%
)

(for /F "tokens=*" %%a in (%file2%) do (
    set line=%%a
    echo !line! | findstr /C:"%search_str%" >nul
    if !errorlevel! == 0 (
        set "line=!line:%search_str%=%replace_str%!"
    )
    echo !line!
)) > temp2.txt

move /Y temp2.txt %file2%

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

cd ..

cd application
cd src

REM Revert the changes to GCVisualizer.hpp and GCInvokeDisplaySingleton.hpp
copy /Y GCVisualizerTEMP.hpp %file%
copy /Y GCInvokeDisplaySingletonTEMP.hpp %file2%
del GCVisualizerTEMP.hpp
del GCInvokeDisplaySingletonTEMP.hpp

cd ..
cd ..

rem Navigate to the application directory
cd application

rem Copy the GCDisplay.py file from the src directory to the bin directory
copy src\GCDisplay.py bin\

rem Navigate back to the root directory
cd ..
