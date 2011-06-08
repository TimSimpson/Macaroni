id =
{
    group="Macaroni",
    name="Macaroni.All",
    version="0.1.0.15",
    author="Tim Simpson"
}

description= [[  
    Builds the development version of Macaroni named "Cavatappi" before running all tests.
    Rebuilds Macaroni from pure C++ source to ensure it is identical, then creates the
    Macaroni website.
]]


children =
{
  "Main",
  --"Generators",
  --"Libraries"  
  "Tests",
  "Release",
};
        
function generate()    
end
   	
function build()	
  
end

function install()	
end
