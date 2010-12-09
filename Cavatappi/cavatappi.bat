@ECHO OFF
REM Calls the current development version of Macaroni. 
REM Named "Cavatappi" to distinguish it from the "trusted" version of Macaroni.
REM Expects a variable named "CAVATAPPI" to be defined.
ECHO ~ CAVATAPPI! ~
SET ddir=%CAVATAPPI%\..\Macaroni\Macaroni\Source
SET rdir=%CAVATAPPI%\..\Macaroni\Macaroni\release\debug

CALL %CAVATAPPI%\refreshc.bat

@ECHO ON
%rdir%\macaroni.exe generate %*


