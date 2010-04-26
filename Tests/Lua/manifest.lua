require "os"
require "Macaroni.Model.Library"

id =
{
    group="Macaroni",
    name="Regression Tests- Lua Glue",
    version="1.0.0.0"
}

description="An example Test Project to be compiled by Macaroni."

source "Source"
output = "GeneratedSource"

dependency {group="Macaroni", name="Boost", version="1.39.0-0"}
dependency {group="Macaroni", name="CppStd", version="1.0.0.0"}


function generate()
    print "A call was made to GENERATE!!!\n\n"
    runGenerator "InterfaceMh"
    runGenerator "Cpp"
    runGenerator "JamGenerator"
    local rtnCode = os.execute("bjam")
    print("BJAM return code = " .. rtnCode .. ".")
    if (rtnCode ~= 0) then
        error("Looks like the plan's not going swell.")
    end
end


