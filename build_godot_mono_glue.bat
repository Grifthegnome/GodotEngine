@echo on
cd /d . REM Sets directory to local file dir.
start bin/godot.windows.editor.x86_64.mono.exe --headless --generate-mono-glue modules/mono/glue 

pause