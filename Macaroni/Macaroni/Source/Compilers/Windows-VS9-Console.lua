require "Windows-VS9"

additionalCompilerArgs=additionalCompilerArgs .. " /MDd "


additionalLinkerArgs=additionalLinkerArgs .. 
[[/SUBSYSTEM:CONSOLE /DYNAMICBASE /NXCOMPAT /MACHINE:X86 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib]]

preprocessorDirectives["_CONSOLE"] = ""
