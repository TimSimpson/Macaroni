upper = getUpperLibrary();
id =
{
    group=upper.Group;
    name="Macaroni.Main",
    version=upper.Version,
    author="Tim Simpson"
}

description= [[  
    Builds development version of Macaroni binary and support files and Cavatappi script to access it.
]]

-- Bug- the Boost Build builder only looks at the last source path.
sources = { "CavatappiGenerators" }

--properties.macaroni = nil or {};
--properties.macaroni.releasepath = "/Release";

children =
{
  "Dependencies",
  "App",
  --"Generators",
  --"Libraries"
  "Cavatappi"
};
        
function generate()    
end
   	
function build()	
  
end

function install()	
end
