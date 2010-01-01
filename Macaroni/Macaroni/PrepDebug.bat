mkdir "Debug"
mkdir "Debug\Compilers"
xcopy "Source\Compilers" ".\Debug\Compilers" /y /s /e
mkdir "Debug\Generators"
xcopy "Source\Generators" ".\Debug\Generators" /y /s /e
xcopy "Source\Messages.txt" ".\Debug\Messages.txt" /y /s /e
"C:\Program Files\Microsoft Visual Studio 9.0\VC\vcvarsall.bat"
PATH=%PATH%;F:\Lp3\Projects\Macaroni\Code\trunk\Macaroni\Macaroni\Debug