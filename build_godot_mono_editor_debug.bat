@echo off
cd /d . REM Sets directory to local file dir.
scons platform=windows target=editor dev_build=yes module_mono_enabled=yes
pause