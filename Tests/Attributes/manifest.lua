require "os"
require "Macaroni.Model.Library"

id =
{
    group="Macaroni",
    name="Regression Tests- Attribute Example",
    version="1.0.0.0"
}

-- Life cycle:
-- Read file.
-- For each dependency, load manifest and parse.
-- Take id and use it to create the current library.
-- For all sources defined, call Macaroni parser to create model.
-- call default, which is defined to be "generate"

description=[[
This example puts Attributes on various nodes.
]]

source "Source"
output = "GeneratedSource"

dependency {group="Macaroni", name="Boost", version="1.39.0-0"}
dependency {group="Macaroni", name="CppStd", version="1.0.0.0"}

function generate()
    run "AttributeReader"   
end

function build()
end

function install()
end
