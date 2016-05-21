@echo off
macaroni --messagesPath Source\main\resources\Messages.txt --libraryRepoPath=..\Libraries  -b  --showPaths -o "link=static --toolset=msvc-12.0 -j8 address-model=32 release" --printTimes --verbose
