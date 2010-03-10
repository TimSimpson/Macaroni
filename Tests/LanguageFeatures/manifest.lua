require "os"
require "Macaroni.Model.Library"

id =
{
    group="Macaroni",
    name="Regression Tests- Language Features",
    version="1.0.0.0"
}

-- Life cycle:
-- Read file.
-- For each dependency, load manifest and parse.
-- Take id and use it to create the current library.
-- For all sources defined, call Macaroni parser to create model.
-- call default, which is defined to be "generate"

print "HELLO! This is the Test manifest.  Code can be executed when the Manifest is originally parsed.";
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


