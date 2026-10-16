@echo on
cd /d . REM Sets directory to local file dir.
bin\godot.windows.editor.x86_64.mono.exe --headless --generate-mono-glue modules/mono/glue 

pause