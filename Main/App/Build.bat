echo off
REM ***************************************************************************
REM Check for the presense of tools.
REM ***************************************************************************
REM IF NOT EXIST "bjam.exe" GOTO BJAM_NOT_FOUND
REM IF NOT EXIST "macaroni.exe" GOTO MACARONI_NOT_FOUND

GOTO BUILD

:BJAM_NOT_FOUND
echo Boot Build not found!  Please add this to your PATH.
GOTO THE_END

:MACARONI_NOT_FOUND
echo Macaroni not found!  Please add this to the PATH.
GOTO THE_END

:BUILD
setlocal enabledelayedexpansion
echo on
macaroni generate
if "%ERRORLEVEL%"=="0" bjam -d+2
bjam -d+2

:THE_END
echo on
