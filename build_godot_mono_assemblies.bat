@echo off
cd /d %~dp0 REM Sets directory to batch file dir.

REM Tell NuGet to use this directory.
dotnet nuget remove source GodotNugetSource
mkdir GodotNugetSource 2>nul
dotnet nuget add source %cd%\GodotNugetSource --name GodotNugetSource

python ./modules/mono/build_scripts/build_assemblies.py --godot-output-dir ./bin --push-nupkgs-local %cd%/GodotNugetSource

pause