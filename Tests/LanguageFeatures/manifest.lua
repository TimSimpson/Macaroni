require "os"

id =
{
    group="TS4",
    name="Test Project",
    version="1.0.0.0"
}

-- Life cycle:
-- Read file.
-- For each dependency, load manifest and parse.
-- Take id and use it to create the current library.
-- For all sources defined, call Macaroni parser to create model.
-- call default, which is defined to be "generate"

description="An example Test Project to be compiled by Macaroni."

source "Source"
output = "GeneratedSource"

dependency {group="Macaroni", name="Boost", version="1.39.0-0"}
--dependency {group="Macaroni", name="CppStd", version=LATEST}


function generate()
    runGenerator "Cpp"
    runGenerator "JamGenerator"
end

function default()
    generate()
    os.execute("bjam")
end

