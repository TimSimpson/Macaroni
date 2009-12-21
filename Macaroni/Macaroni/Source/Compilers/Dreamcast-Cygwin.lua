compilerExe="C:\cygwin\usr\local\dc\sh-elf\bin\sh-elf-g++.exe"
oSwitch="-o"
additionalCompilerArgs="-O2 -DFRAME_POINTERS -ml -m4-single-only -fno-optimize-sibling-calls -D_arch_dreamcast -D_arch_sub_pristine -Wall-g -fno-builtin -fno-strict-aliasing -ml -m4-single-only -Wl,-Ttext=0x8c010000-nostartfiles -nostdlib -DCOMPILE_TARGET_DREAMCAST "
additionalLinkerArgs="-g -fno-builtin -fno-strict-aliasing -ml -m4-single-only -Wl,-Ttext=0x8c010000 -nostartfiles -nostdlib -Wall-g -lstdc++ -Wl,--start-group -lkallisti -lc -lgcc -Wl,--end-group -lgl -lpcx -lkosutils -lm -mwindows -g3 -lconio"
firstLinkedObjects=" C:/Cygwin/usr/local/dc/kos/kos/kernel/arch/dreamcast/kernel/startup.o"
paths=
{
    "C:\Cygwin\bin",
    "C:\Cygwin\usr\local\dc\kos\kos\include",
    "C:\Cygwin\usr\local\dc\kos\kos\kernel\arch\dreamcast\include",
    "C:\Cygwin\usr\local\dc\kos\kos\addons\include",
    "C:\Cygwin\usr\local\dc\sh-elf\sh-elf\include",
    "C:\Cygwin\usr\local\dc\sh-elf\include\c++\3.4.2",
    "C:\Cygwin\usr\local\dc\kos\kos\lib\dreamcast",
    "C:\Cygwin\usr\local\dc\kos\kos\addons\lib\dreamcast",
    "C:\Cygwin\usr\local\dc\sh-elf"    
}
environmentVariables=
{
    "INCLUDE"="C:\Program Files\Microsoft Visual Studio 9.0\VC\ATLMFC\INCLUDE;C:\Program Files\Microsoft Visual Studio 9.0\VC\INCLUDE;C:\Program Files\Microsoft SDKs\Windows\v6.0A\include"
}
preprocessorDirectives=
{
   "WIN32"="",
   "_DEBUG"="",
   "_WINDOWS"="",
   "COMPILE_TARGET_WINDOWS"="",
   "COMPILE_USETOOLS"="" 
}
includePaths=
{
    "/usr/local/dc/kos/kos/include",
    "/usr/local/dc/kos/kos-ports/include",
    "/usr/local/dc/kos/kos/kernel/arch/dreamcast/include",
    "/usr/local/dc/kos/kos/addons/include",
    "/usr/local/dc/sh-elf/sh-elf/include",
    "/usr/local/dc/sh-elf/include/c++/3.4.2"    
}
linkerPaths=
{
    "C:/Cygwin/usr/local/dc/kos/kos/lib/dreamcast",
    "C:/Cygwin/usr/local/dc/kos/kos/addons/lib/dreamcast",
    "C:/Cygwin/usr/local/dc/sh-elf"
}