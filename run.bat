@ECHO OFF

:t
cls
ECHO Building...
make clean
make
rmdir /Q /S build
if exist "3DS-Cookie-Clicker.3dsx" (
    del 3DS-Cookie-Clicker.elf
    del 3DS-Cookie-Clicker.smdh
    ECHO Done! Launching...
    3DS-Cookie-Clicker.3dsx
) else (
    ECHO Failed to build! Check Errors.
)
del 3DS-Cookie-Clicker.3dsx
pause
goto t