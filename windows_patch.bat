@echo off
setlocal enabledelayedexpansion

set apply_lasi=false
set apply_webp=false
set revert=false

:parse_flags
if "%1" == "-lasi" (
    set apply_lasi=true
    shift
    goto :parse_flags
) else if "%1" == "-webp" (
    set apply_webp=true
    shift
    goto :parse_flags
) else if "%1" == "-revert" (
    set revert=true
    shift
    goto :parse_flags
) else if not "%1" == "" (
    echo Unrecognized flag: %1
    echo Usage: apply_changes.bat [-lasi] [-webp] [-revert]
    exit /b 1
)

cd application\include\graphviz

if %revert% == true (
    call :revert_plugin_changes lasi
    call :revert_plugin_changes webp
) else (
    if %apply_lasi% == true (
        call :apply_plugin_changes lasi gvplugin_lasi
        goto :plugin_changes_applied
    )

    if %apply_webp% == true (
        call :apply_plugin_changes webp gvplugin_webp
        goto :plugin_changes_applied
    )
)

:plugin_changes_applied
exit /b 0

:apply_plugin_changes
set plugin_name=%1
set target_name=%2
echo Applying changes to %plugin_name% plugin...

echo Modifying cmd/dot/CMakeLists.txt...
type "cmd\dot\CMakeLists.txt" > temp.txt
findstr /V /C:"    target_link_libraries(dot_builtins PRIVATE %target_name%)" temp.txt > cmd/dot/CMakeLists.txt
del temp.txt

echo Modifying plugin/CMakeLists.txt...
type "plugin\CMakeLists.txt" > temp.txt
findstr /V /C:"add_subdirectory(%plugin_name%)" temp.txt > plugin/CMakeLists.txt
del temp.txt

echo Changes applied successfully for %plugin_name% plugin.
goto :eof

:revert_plugin_changes
set plugin_name=%1
echo Reverting changes to %plugin_name% plugin...
git reset --hard HEAD
echo Changes reverted successfully for %plugin_name% plugin.
goto :eof