id =  -- Not needed.
{    
    group="Macaroni",
    name="Macaroni",
    version="1.0.0.0",
    author="Tim Simpson"
}

description=
[[
    ~~     ~~                                     ~
   ~~ ~~ ~~ ~~   ~~ ~~   ~~  ~~~    ~~~   ~  ~         ~~  ~
  ~~   ~~  ~~  ~  ~    ~  ~  ~  ~ ~   ~  ~ ~ ~  ~    ~   ~~ 
 ~~       ~~    ~~  ~~  ~~  ~      ~~~  ~   ~  ~      
    Macaroni is a builder / parser for C++ that aims to increase
    DRYness by slimming down boilerplate and eliminating the need to store
    redundant information.
]]

mSource = 
{  
    "Source"
}
mOutput = "GeneratedSource"

cppInput =  
{
    "MWork/GeneratedSource",
    "Source"
}

cppOutput = {
    headers="MWork/Headers",
    objects="MWork/Objects",
    cppSource="PureCppBackup"
}


fOutput = "MWork/Final"

configurations = {
    all =
    {
        compiler = "Windows-VS9-Console",        
        generators =
        {
            "Cpp.lua",       
            "LuaGlue.lua",
            "InterfaceMh.lua",              
        },
        dependencies =
        {
            boost = {
                group="Macaroni",
                name="Boost",
                version="1.39.0-0",
            },
            std = {
                group="Macaroni",
                name="CppStd",
                version="1.0.0.0",
                --configuration="all" -- <--  todo: determine how this will work.
            }
        },
        final = "Macaroni.exe"
    }
}