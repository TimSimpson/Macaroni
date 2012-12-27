require "Macaroni.IO.Path";
Path = Macaroni.IO.Path;

upper = getUpperLibrary();
id =
{
    group=upper.Group,
    name="Macaroni.Release",
    version=upper.Version,
    author="Tim Simpson"
}

description= [[  
    Bundles Macaroni distribution files and the creates the Macaroni website.
]]

cavatappi("cavatappi.lua");