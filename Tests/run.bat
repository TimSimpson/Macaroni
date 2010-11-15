@ECHO OFF
SET TEST_MACARONI

ECHO Yo
IF NOT EXIST "%TEST_MACARONI_EXE%" GOTO NotFound
GOTO Start

ECHO Yo2
:NotFound
ECHO Yo4
SET TEST_MACARONI_EXE="..\Macaroni\Macaroni\release\debug\macaroni.exe"
ECHO Yo5
ECHO The environment variable TEST_MACARONI_EXE must be set for this command to execute.  Setting to value %TEST_MACARONI_EXE%.

ECHO Yo3
:Start
CALL :RunTest LanguageFeatures

GOTO TheEnd

:RunTest
ECHO HAI
SET TEST_DIR = %1
CD "%TEST_DIR%"
"%TEST_MACARONI_EXE%" generate
CD ..
GOTO:EOF


:TheEnd
@ECHO ON