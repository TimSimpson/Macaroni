set MACARONI_CODE_ROOT=%CD%

set MACARONI_EXE=%MACARONI_CODE_ROOT%\Macaroni\Debug\Macaroni.exe
set MACARONI_EXE_DIRECTORY=%MACARONI_CODE_ROOT%\Macaroni\Debug
set MACARONI_CODE_SOURCE=%MACARONI_CODE_ROOT%\Macaroni\Macaroni\Source

echo Copying files into Macaroni EXE directory.
copy "%MACARONI_CODE_SOURCE%\Messages.txt" "%MACARONI_EXE_DIRECTORY%\Messages.txt"

mkdir "%MACARONI_EXE_DIRECTORY%\Generators"

xcopy "%MACARONI_CODE_SOURCE%\Generators" "%MACARONI_EXE_DIRECTORY%\Generators" /y /s /e

echo Running Macaroni from path %MACARONI_EXE%.
echo

pushd %MACARONI_CODE_SOURCE%
"%MACARONI_EXE%" -luaTests "%MACARONI_CODE_SOURCE%
popd
