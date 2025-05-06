@ECHO OFF

:t
cls
ECHO Building...
make clean
make
rmdir /Q /S build
if exist "application.3dsx" (
    del application.elf
    del application.smdh
    ECHO Done! Launching...
    application.3dsx
) else (
    ECHO Failed to build! Check Errors.
)
pause
del application.3dsx
goto t