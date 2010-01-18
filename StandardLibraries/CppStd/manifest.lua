id = 
{
    group="Macaroni",
    name="CppStd",
    version="1.0.0.0"
}

description=
[[Defines a Macaroni interface as well as include paths for the C++
standard library.]]

mSource = 
{  
    "Source"
}
mOutput = "MWork/GeneratedSource"

cppInput =  
{ 
    "MWork/GeneratedSource",
    "Source"
}

cppOutput = {
    objects="MWork/Objects",
}

fOutput = "MWork/Final"

configurations = {
    all =
    {
        compiler = "skip",        
        generators =
        {
            "Cpp.lua",       
            "InterfaceMh.lua"                 
        },
        dependencies =
        {
            -- No dependencies yet exist, but if they did we'd put 'em here like this:
            -- localName = module("name","version","configuration")
        },
        final = "Test.exe"
    }   
}