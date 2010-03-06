echo off
REM ***************************************************************************
REM Establish machine specific paths.
REM Shouldn't have to set BOOST_BUILD_PATH as it will find the files in my home directory.
REM ***************************************************************************
set BOOST_ROOT=F:\Tools\boost_1_42_0
set BJAM_EXE=F:\Tools\boost_1_42_0\bjam.exe
REM set BOOST_BUILD_PATH=F:\Lp3

REM ***************************************************************************
REM Print out some sanity checks:
REM ***************************************************************************

echo ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
echo BUILDING MACARONI FROM PURE C++ SOURCES...
echo   BOOST_ROOT      :%BOOST_ROOT%
echo   BJAM_EXE        :%BJAM_EXE%
REM echo   BOOST_BUILD_PATH:%BOOST_BUILD_PATH%
echo ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

REM ***************************************************************************
REM Invoke Boost.Build, vicariously through BJam.  The %* contains all the 
REM arguments passed to this file.
REM The "-d+2" argument specifies more verbose output to help identify errors.
REM ***************************************************************************
echo on
%BJAM_EXE% -d+2 %*
