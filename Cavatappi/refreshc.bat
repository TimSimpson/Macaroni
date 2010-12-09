SET ddir=%CAVATAPPI%\..\Macaroni\Macaroni\Source
SET rdir=%CAVATAPPI%\..\Macaroni\Macaroni\release\debug

CALL :CopyL "Generators\BoostBuild.lua"
CALL :CopyL "Generators\Cpp\ClassCppFileGenerator.lua"
CALL :CopyL "Generators\Cpp.lua"
CALL :CopyL "Generators\Cpp\CppFileGenerator.lua"
CALL :CopyL "Generators\Cpp\HFileGenerator.lua"
CALL :CopyL "Generators\Cpp\NodeFileGenerator.lua"
CALL :CopyL "Generators\HtmlView.lua"
CALL :CopyL "Generators\InterfaceMh.lua"
CALL :CopyL "Generators\LuaGlue.lua"
CALL :CopyL "Generators\Macaroni\MhFileGenerator.lua"

CALL :CopyL "Libraries\Macaroni\Boost-filesystem\1.42.0\Source\Interface.mh"
CALL :CopyL "Libraries\Macaroni\Boost-filesystem\1.42.0\manifest-final.lua"
CALL :CopyL "Libraries\Macaroni\Boost-filesystem\1.42.0\Source\PrepareBoostBuild.lua"

CALL :CopyL "Libraries\Macaroni\Boost-smart_ptr\1.42.0\Source\Interface.mh"
CALL :CopyL "Libraries\Macaroni\Boost-smart_ptr\1.42.0\manifest-final.lua"
CALL :CopyL "Libraries\Macaroni\Boost-smart_ptr\1.42.0\Source\PrepareBoostBuild.lua"

CALL :CopyL "Libraries\Macaroni\CppStd\2003\Source\Interface.mh"
CALL :CopyL "Libraries\Macaroni\CppStd\2003\manifest-final.lua"


GOTO THE_END

:CopyL
SET src=%ddir%\%1
SET dst=%rdir%\%1
ECHO %1
copy %src% %dst%
GOTO:EOF

:THE_END