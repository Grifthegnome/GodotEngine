@echo off
cd /d "." REM Sets directory to local file dir.
scons platform=windows dev_build=yes dev_mode=yes
pause