local Path = require "Macaroni.IO.Path"


project = context:Group("Macaroni")
                 :Project("Boost-program_options")
                 :Version("1.58");

function errorMessage(msg)
    msg = [[Could not find the value ]] .. msg .. [[ in the global ]] ..
          [[properties. These are set in the init.lua script that lives in ]] ..
          [[the Macaroni home directory.]]
    output:ErrorLine(msg)
    error(msg)
end


if (properties.boost == nil) then
    errorMessage('boost')
end
local boost_props = properties.boost["1.58"]
if (boost_props == nil) then
    errorMessage(properties.boost.version)
end

lib = project:Library
{
    name="lib",
    sources = pathList{"Source"},
}
local headers = lib.Headers
require "Macaroni.IO.Path"

headers[#headers + 1] = Path.New(boost_props.include)

function install()
    sinstall(project, filePath("./"));
end
