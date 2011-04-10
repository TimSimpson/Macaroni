ECHO Adding Boost to path.
IF %1.==. GOTO MissingBoost
IF %2.==. GOTO MissingMacaroni
goto Start

:MissingBoost
ECHO Argument 1 should be the path to the root of the Boost libraries (where BJAM and Boost Build live).
exit 1

:MissingMacaroni
ECHO Argument 2 should be the path to the pre-existing version of Macaroni used to create this build.
exit 1

:Start

SET BOOST_ROOT=%1
SET MACARONI_ROOT=%2
SET PATH=%PATH%;%BOOST_ROOT%
ECHO Adding Macaroni to path.
SET PATH=%PATH%;%MACARONI_ROOT%
cd Main
macaroni clean install
If Errorlevel 0 Goto RunTests
exit 1 

:RunTests
ECHO Adding Cavatappi to path.
SET PATH=%PATH%;%WORKSPACE%\Main\Cavatappi
cd ..
cd Tests
macaroni clean install
If Errorlevel 0 Goto RunRelease
exit 1 

:RunRelease
cd ..
cd Release
macaroni clean install
If Errorlevel 0 Goto Finish
exit 1

:Finish 
cd ..
ECHO All phase successful.
exit 0