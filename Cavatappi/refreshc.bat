
SET rdir=%CAVATAPPI%\..\Libraries\
SET ddir=%CAVATAPPI%\target\Libraries\


MKDIR %CAVATAPPI%\target
MKDIR %ddir%

CALL :CopyL "Macaroni\Boost-filesystem\1.42.0\Source\Interface.mh"
CALL :CopyL "Macaroni\Boost-filesystem\1.42.0\manifest-final.lua"
CALL :CopyL "Macaroni\Boost-filesystem\1.42.0\Source\PrepareBoostBuild.lua"

CALL :CopyL "Macaroni\Boost-smart_ptr\1.42.0\Source\Interface.mh"
CALL :CopyL "Macaroni\Boost-smart_ptr\1.42.0\manifest-final.lua"
CALL :CopyL "Macaroni\Boost-smart_ptr\1.42.0\Source\PrepareBoostBuild.lua"

CALL :CopyL "Macaroni\CppStd\2003\Source\Interface.mh"
CALL :CopyL "Macaroni\CppStd\2003\manifest-final.lua"


GOTO THE_END

:CopyL
SET src=%ddir%\%1
SET dst=%rdir%\%1
ECHO %1
copy %src% %dst%
GOTO:EOF

:THE_END