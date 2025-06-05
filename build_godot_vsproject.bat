@echo off
cd /d "." REM Sets directory to local file dir.
scons platform=windows vsproj=yes vsproj_gen_only=yes dev_build=yes