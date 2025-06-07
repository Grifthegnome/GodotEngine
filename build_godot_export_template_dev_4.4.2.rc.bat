@echo off
cd /d . REM Sets directory to local file dir.
set version=4.4.2.rc
scons platform=windows target=template_debug dev_build=yes dev_mode=yes

mkdir %USERPROFILE%\AppData\Roaming\Godot\export_templates\%version%
rename bin\godot.windows.template_debug.dev.x86_64.exe windows_debug_x86_64.exe
rename bin\godot.windows.template_debug.dev.x86_64.console.exe windows_debug_x86_64_console.exe
move bin\windows_debug_x86_64.exe %USERPROFILE%\AppData\Roaming\Godot\export_templates\%version%
move bin\windows_debug_x86_64_console.exe %USERPROFILE%\AppData\Roaming\Godot\export_templates\%version%

pause