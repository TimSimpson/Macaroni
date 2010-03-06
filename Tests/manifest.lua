id = 
{
    group="TS4",
    name="Test Project",
    version="1.0.0.0"
}

description="An example Test Project to be compiled by Macaroni."

mSource = 
{   -- Ditto.  This is also the default.
    "Source"
}
mOutput = "GeneratedSource"


configurations = {
    all =
    {        
        generators =
        {
            "Cpp.lua",       
            "InterfaceMh.lua"     
            --"SimpleGenerator.lua"
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
        final = "Test.exe"
    }
    -- Need to have some kind of inheretence mechanism for configurations, so
    -- most configs will inhereit from "all" by default.
}

-- Macaroni command line requires path to manifest file (this file) as well
-- as the configuration to build.
-- Or it will look for manifest.lua in the current directory.
-- Generators are looked for first in the current directory, then in the 
-- Generators directory.
-- Macaroni is installed to the C:/Program Files/Macaroni"
-- but work directories are installed to "C:/Users/Tim/Macaroni."
-- 
-- The directories there are "Work" - where files are stored for building -
-- as well as 