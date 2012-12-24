echo off
REM If built with Boost build, use this:
REM %CAVATAPPI%\..\App\GeneratedSource\release\debug\macaroni generatorPath %CAVATAPPI%\..\Generators libraryRepoPath %CAVATAPPI%\LibrariesClone cavatappi %*
REM If built with VS2008, use this:
REM %CAVATAPPI%\..\App\Debug\Macaroni.App generatorPath %CAVATAPPI%\..\Generators libraryRepoPath %CAVATAPPI%\..\Libraries messagesPath %CAVATAPPI%\..\App\Source\main\resources cavatappi %*
REM If built with VS11, use this:
echo on
SET GEN_PATH=%CAVATAPPI%\..\Generators
echo GEN_PATH=%GEN_PATH%
dir %GEN_PATH%
REM %CAVATAPPI%\..\App\Debug\MacaroniVS11.exe generatorPath %GEN_PATH% libraryRepoPath %CAVATAPPI%\..\Libraries messagesPath %CAVATAPPI%\..\App\Source\main\resources cavatappi %*
REM %CAVATAPPI%\..\App\bin\gcc-mingw-4.7.2\debug\link-static\threading-multi\macaroni.exe generatorPath %GEN_PATH% libraryRepoPath %CAVATAPPI%\..\Libraries messagesPath %CAVATAPPI%\..\App\Source\main\resources cavatappi %*
%CAVATAPPI%\..\App\GeneratedSource\exe\macaroni.exe generatorPath %GEN_PATH% libraryRepoPath %CAVATAPPI%\..\Libraries messagesPath %CAVATAPPI%\..\App\Source\main\resources cavatappi %*
