upper = getUpperLibrary();
id =
{
    group=upper.Group,
    name="Macaroni.Release",
    version=upper.Version,
    author="Tim Simpson"
}

description= [[  
    Bundles Macaroni distribution files and the creates the Macaroni website.
]]

sources = { "source/scripts", "source/www" }
output = "target"
   	
function build()	
  run("Site");
end
