@echo off
cd /d . REM Sets directory to local file dir.
call build_godot_mono_editor.bat
call build_godot_mono_glue.bat
call build_godot_mono_assemblies.bat
pause