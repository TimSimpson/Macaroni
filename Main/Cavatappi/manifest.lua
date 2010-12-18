id =
{
    group="Macaroni",
    name="Macaroni-Cavatappi",
    version="0.1.0.6",
    author="Tim Simpson"
}

description= [[  
    Bundles development version of Macaroni nicknamed "Cavatappi."
]]

-- Bug- Macaroni only includes the last Source in its call to the
-- generators, even though it passes in an array.
sources = 
{ 
	"Source",
	"../Libraries", 	
}
output = "target"
        
function generate()    
end
   	
function build()	
  run("CreateCavatappi");
end

function install()	
end
