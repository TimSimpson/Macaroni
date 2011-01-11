require "Macaroni.Generator.Output.Path";
require "Macaroni.Model.Node";
require "Macaroni.Model.Context";
require "Log"

local Context = Macaroni.Model.Context;

function Generate(context, path)
	log.Init("Dumb");
    log:Write "Generating something, I guess.\n";
    log:Write("Path type is " .. tostring(path) .. ".");
    
    local paths = path.Paths;
    log:Write(tostring(paths));
    local length = #paths;
    
    local path1 = paths[1];
    
    log:Write(tostring(path1));
    
    for i = 1, #paths do
        local p = paths[i];        
        log:Write(i .. "=" .. tostring(p));
        if (p.IsDirectory) then 
           log:Write("is a dir.");
        else
            log:Write("is NOT a dir."); 
        end
    end
    --local context = Context.New("{ROOT}", "{WILDCARD}");
    --log:Write('Yet, the type of a Context is "' .. type(context) .. '".');
    --log:Write("The Path is " .. path .. "? No?\n");
    
end