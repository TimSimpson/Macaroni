id =
{
    group="Macaroni",
    name="Macaroni-Cavatappi",
    version="0.1.0.6",
    author="Tim Simpson"
}

description= [[  
    Bundles development version of Macaroni nicknamed "Cavatappi."
    In order to make this work, and to use the resulting "cavatappi" version,
    you need to create an environment variable called "CAVATAPPI" and point
    it to the same directory as this manifest.
]]

-- Bug- Macaroni only includes the last Source in its call to the
-- generators, even though it passes in an array.
sources = 
{ 
	--"Source"
	--"""../Libraries", 	
}
output = "target"
        
function generate()    
end
   	
function build()	
  --run("CreateCavatappi");
  -- UGH.  I hate myself for doing it like this:
  local cavatappiVar = os.getenv("CAVATAPPI");
  if (cavatappiVar == nil) then
  	error([[You must set an environment variable CAVATAPPI with the path to the]] ..
  	      [[ Main/Cavatappi to use Cavatappi.]])
  end
  os.execute([[mkdir %CAVATAPPI%\LibrariesClone]]);
  os.execute([[xcopy %CAVATAPPI%\..\Libraries %CAVATAPPI%\LibrariesClone /S /C /Y]]);  
end

function install()	
end
