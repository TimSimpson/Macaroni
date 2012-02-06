project = context:Group("Macaroni")
                 :Project("Boost-headers")
                 :Version("1.46.1");

if (properties.boost == nil
    or properties.boost["1.46.1"] == nil) then
    output:ErrorLine(
[[Location of the Boost libraries is unknown. Please specify the variable ]]
.. [["properties.boost["1.46.1"] in the init.lua script.]]);
end
boost_props = properties.boost["1.46.1"]

lib = project:Library
{
    name="lib",
    sources = files{
        Macaroni = { fileSet("Source", ".mh$") }
    }
}
headers = lib:AddArtifact("headers")
headers.Files:Add(fileSet(boost_props.include, ""));
