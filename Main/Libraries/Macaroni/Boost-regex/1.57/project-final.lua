Path = require "Macaroni.IO.Path"

project = context:Group("Macaroni")
                 :Project("Boost-regex")
                 :Version("1.57");

if (properties.boost == nil
    or properties.boost["1.57"] == nil) then
    output:ErrorLine(
[[Location of the Boost libraries is unknown. Please specify the variable ]]
.. [["properties.boost["1.57"] in the init.lua script.]]);
end
boost_props = properties.boost["1.57"]

lib = project:Library
{
    name="lib",
    sources = pathList{"Source"},
}
local headers = lib.Headers
require "Macaroni.IO.Path"

headers[#headers + 1] = Path.New(boost_props.include)
