project = context:Group("Macaroni")
                 :Project("Boost-headers")
                 :Version("1.51");

if (properties.boost == nil
    or properties.boost["1.51"] == nil) then
    output:ErrorLine(
[[Location of the Boost libraries is unknown. Please specify the variable ]]
.. [["properties.boost["1.51"] in the init.lua script.]]);
end
boost_props = properties.boost["1.51"]

lib = project:Library
{
    name="lib",
    sources = pathList{"Source", ".mh$"},
}
--headers = lib:AddArtifact("headers")
--headers.Files:Add(fileSet(boost_props.include, ""));
local headers = lib.Headers
require "Macaroni.IO.Path"

headers[#headers + 1] = Macaroni.IO.Path.New(boost_props.include)
