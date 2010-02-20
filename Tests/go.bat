mkdir "..\Macaroni\Macaroni\Debug\Compilers"
xcopy "..\Macaroni\Macaroni\Source\Compilers" "..\Macaroni\Macaroni\\Debug\Compilers" /y /s /e
mkdir "..\Macaroni\Macaroni\Debug\Generators"
xcopy "..\Macaroni\Macaroni\Source\Generators" "..\Macaroni\Macaroni\\Debug\Generators" /y /s /e
xcopy "..\Macaroni\Macaroni\Source\Messages.txt" "..\Macaroni\Macaroni\\Debug\Messages.txt" /y /s /e

echo ..\Macaroni\Macaroni\Debug\Macaroni.exe generate saveCppSource compile install


