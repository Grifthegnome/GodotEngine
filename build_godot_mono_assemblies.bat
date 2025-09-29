@echo off
cd /d . REM Sets directory to local file dir.

REM Generate our NuGet directory if we don't have one. 
IF NOT EXIST "./localNuGet" (

	mkdir "./localNuGet"
)

REM Tell NuGet to use this directory.
dotnet nuget add source "./localNuGet" --name MyLocalNugetSource

python ./modules/mono/build_scripts/build_assemblies.py --godot-output-dir ./bin --push-nupkgs-local "./localNuGet"

pause