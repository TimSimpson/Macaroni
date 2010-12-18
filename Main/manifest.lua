id =
{
    group="Macaroni",
    name="Macaroni-Main",
    version="0.1.0.6",
    author="Tim Simpson"
}

description= [[  
    Builds development version of Macaroni binary and support files and Cavatappi script to access it.
]]

-- Bug- the Boost Build builder only looks at the last source path.
sources = { "CavatappiGenerators" }
output = "Cavatappi"

--properties.macaroni = nil or {};
--properties.macaroni.releasepath = "/Release";

children =
{
  "App",
  "Generators",
  "Libraries"
};
        
function generate()    
end
   	
function build()	
  run("CreateCavatappi");
end

function install()	
end
