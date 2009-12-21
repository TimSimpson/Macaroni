id = 
{
    group="TS4",
    name="Test Project",
    version="1.0.0.0"
}

description="An example Project to be compiled by Macaroni."

cInclude =  -- These include paths are the default for a manifest, I'm putting it here to illustrate it.
{   -- This refers to the Includes you'd give the C++ compiler, btw. Macaroni Compiler does not use it.
    "MWork/GeneratedSource",
    "Source"
}
mSource = 
{   -- Ditto.  This is also the default.
    "Source"
}
mOutput = "MWork/GeneratedSource"

configurations = {
    all =
    {
        compiler = "default",
        generators =
        {
            "SimpleGenerator.lua"
        },
        dependencies =
        {
            -- No dependencies yet exist, but if they did we'd put 'em here like this:
            -- localName = module("name","version","configuration")
        }    
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