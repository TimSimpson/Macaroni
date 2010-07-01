id =
{
    group="Macaroni",
    name="Macaroni",
    version="0.1.0.5",
    author="Tim Simpson"
}

description= [[  
    Macaroni is a builder / parser for C++ that aims to increase
    DRYness by slimming down boilerplate and eliminating the need to store
    redundant information.
]]

source "Source"
output = "GeneratedSource"

dependency { group="Macaroni", name="Boost", version="1.39.0-0" }
dependency { group="Macaroni", name="CppStd", version="1.0.0.0" }
    
function bjam()
    --local rtnCode = os.execute("bjam")
    return rtnCode == 0
end
        
function generate()    
    runGenerator "Cpp"
    --runGenerator "Generators/LuaGlue"
    runGenerator "InterfaceMh"
    runGenerator "JamGenerator"
    runGenerator "VersionInfoGenerator"
    print "Code Generation successful.  Calling Boost Build."
    if (bjam()) then
        print "~ YOU WIN! ~"
    else
        print "~ YOU LOSE ~"          
    end
end
   