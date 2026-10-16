@echo off
cd /d . REM Sets directory to local file dir.

REM Tell NuGet to use this directory.
dotnet nuget add source %cd%/GodotNugetSource --name GodotNugetSource

python ./modules/mono/build_scripts/build_assemblies.py --godot-output-dir ./bin --push-nupkgs-local %cd%/GodotNugetSource

pause