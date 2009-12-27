xcopy "..\Macaroni\Macaroni\Source\Generators" "..\Macaroni\Debug\Generators" /y /s /e
xcopy "..\Macaroni\Macaroni\Source\Compilers" "..\Macaroni\Debug\Compilers" /y /s /e
echo Running Macaroni.
REM ..\Macaroni\Debug\Macaroni.exe -inputDirectory Input -outputDirectory Actual
..\Macaroni\Debug\Macaroni.exe


