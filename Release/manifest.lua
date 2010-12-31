id =
{
    group="Macaroni",
    name="Macaroni-Site",
    version="0.1.0.6",
    author="Tim Simpson"
}

description= [[  
    Creates the Macaroni website.
]]

sources = { "source/scripts", "source/www" }
output = "target"
   	
function build()	
  run("Site");
end
