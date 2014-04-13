@echo off
SET GEN_PATH=%CAVATAPPI%\..\Generators
@echo on
%CAVATAPPI%\..\App\GeneratedSource\exe\macaroni.exe --generatorPath=%GEN_PATH% --libraryRepoPath=%CAVATAPPI%\..\Libraries --messagesPath=%CAVATAPPI%\..\App\Source\main\resources\Messages.txt %*
