@ECHO OFF

:: Please set the name of the application.
set APPLICATION=3DS-Cookie-Clicker

:t
cls
ECHO ======================================================================
ECHO Building...
ECHO == COMMAND OUTPUT ====================================================
make clean
make
rmdir /Q /S build
ECHO ======================================================================
if exist %APPLICATION%.3dsx (
    del %APPLICATION%.elf
    del %APPLICATION%.smdh
    ECHO Done! Launching...
    ECHO ======================================================================
    %APPLICATION%.3dsx
    del %APPLICATION%.3dsx
) else (
    ECHO Failed to build! Check Errors.
    ECHO Or check if your APPLICATION variable is the same name as the .3dsx
    ECHO Your VAR: %APPLICATION%
    ECHO ======================================================================
)
pause
goto t