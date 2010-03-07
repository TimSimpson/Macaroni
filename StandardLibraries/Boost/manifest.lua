id = 
{
    group="Macaroni",
    name="Boost",
    version="1.39.0-0"
}

description=
[[The Boost libraries.]]

configurations = {
    all =
    {
        compiler = "skip",  
   
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
   
    }   
}
