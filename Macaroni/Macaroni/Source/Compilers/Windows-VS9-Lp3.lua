require "Windows-VS9"


includePaths[#includePaths + 1] 
    = [[C:\Program Files\Microsoft DirectX SDK (November 2007)\Include]]

-- Explanations of "/MDd" option.
-- http://msdn.microsoft.com/en-us/library/2kzt1wy3%28VS.80%29.aspx
additionalCompilerArgs = additionalCompilerArgs ..
" /MDd"
--"/Gm /RTC1 /W3 /c /ZI /TP "

additionalLinkerArgs = additionalLinkerArgs .. 
"/SUBSYSTEM:WINDOWS /DYNAMICBASE /NXCOMPAT /MACHINE:X86 ";