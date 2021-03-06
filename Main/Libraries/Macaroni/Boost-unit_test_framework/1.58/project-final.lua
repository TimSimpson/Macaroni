local Path = require "Macaroni.IO.Path";


project = context:Group("Macaroni")
                 :Project("Boost-unit_test_framework")
                 :Version("1.58");

if (properties.boost == nil
    or properties.boost["1.58"] == nil) then
    output:ErrorLine(
[[Location of the Boost libraries is unknown. Please specify the variable ]]
.. [["properties.boost["1.58"] in the init.lua script.]]);
end
boost_props = properties.boost["1.58"]

lib = project:Library
{
    name="lib",
    sources = pathList{"Source"},
}
--headers = lib:AddArtifact("headers")
--headers.Files:Add(fileSet(boost_props.include, ""));
local headers = lib.Headers
require "Macaroni.IO.Path"

headers[#headers + 1] = Path.New(boost_props.include)
