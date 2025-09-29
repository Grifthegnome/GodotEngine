@echo off
cd /d . REM Sets directory to local file dir.
python ./modules/mono/build_scripts/build_assemblies.py --godot-output-dir=./bin

pause