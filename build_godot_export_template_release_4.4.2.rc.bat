@echo off
cd /d . REM Sets directory to local file dir.
set version=4.4.2.rc
echo building release template for godot %version%
scons platform=windows target=template_release

mkdir %USERPROFILE%\AppData\Roaming\Godot\export_templates\%version%
rename bin\godot.windows.template_release.x86_64.exe windows_release_x86_64.exe
rename bin\godot.windows.template_release.x86_64.console.exe windows_release_x86_64_console.exe
move bin\windows_release_x86_64.exe %USERPROFILE%\AppData\Roaming\Godot\export_templates\%version%
move bin\windows_release_x86_64_console.exe %USERPROFILE%\AppData\Roaming\Godot\export_templates\%version%

pause