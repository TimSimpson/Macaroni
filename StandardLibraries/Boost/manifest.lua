id = 
{
    group="Macaroni",
    name="Boost",
    version="1.39.0-0"
}

description=
[[The Boost libraries.]]

-- "Settings" refers to special settings files which are stored in the user
-- directory in the "Settings" folder.
-- The premise is that this is where special install / user dependent paths
-- and other things can be stored.  For instance, Boost could be just about
-- anywhere on a user's machine, so to use the boost libraries
-- they have to define the settings file themselves.]
--useSettings("Boost/1.39.0.lua");
require "Boost/1_39_0";
if (boostInstallRootDirectory == nil) then
    error("The global variable \"boostInstallRootDirectory\" was not found.");
end
if (boostLinkerLibraryDirectory == nil) then
    error("The global variable \"boostLinkerLibraryDirectory\" was not found.");
end

configurations = {
    all =
    {
        compiler = "skip",  
        cInclude = {
            boostInstallRootDirectory
        },             
        generators =
        { 
            "InterfaceMh.lua"                 
        },
        dependencies =
        {
            std = 
            {
                group="Macaroni",
                name="CppStd",
                version="1.0.0.0"
            }            
        },
        linkerLibraries = {
            boostLinkerLibraryDirectory
        },
        final = "Test.exe"
    }   
}