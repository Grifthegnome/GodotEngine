@echo off
cd /d . REM Sets directory to local file dir.
set version=4.4.2.rc
echo building release template for godot %version%
scons platform=windows target=template_release module_mono_enabled=yes

mkdir %USERPROFILE%\AppData\Roaming\Godot\export_templates\%version%.mono
rename bin\godot.windows.template_release.x86_64.mono.exe windows_release_x86_64.exe
rename bin\godot.windows.template_release.x86_64.mono.console.exe windows_release_x86_64_console.exe
move bin\windows_release_x86_64.exe %USERPROFILE%\AppData\Roaming\Godot\export_templates\%version%.mono
move bin\windows_release_x86_64_console.exe %USERPROFILE%\AppData\Roaming\Godot\export_templates\%version%.mono

pause