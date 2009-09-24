xcopy "..\Macaroni\Macaroni\Source\Generators" "..\Macaroni\Debug\Generators" /y /s /e
echo Running Macaroni.
..\Macaroni\Debug\Macaroni.exe -inputDirectory Input -outputDirectory Actual

