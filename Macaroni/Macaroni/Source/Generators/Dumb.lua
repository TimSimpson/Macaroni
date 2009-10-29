require "Macaroni.Generator.Output.Path";
require "Macaroni.Model.Node";
require "Macaroni.Model.Context";

local Context = Macaroni.Model.Context;

print "Hello, I am dumb.\n";

function Generate(context, path)
    print "Generating something, I guess.\n";
    print("Path type is " .. tostring(path) .. ".");
    
    local paths = path.Paths;
    print(tostring(paths));
    local length = #paths;
    
    local path1 = paths[1];
    
    print(tostring(path1));
    
    for i = 1, #paths do
        local p = paths[i];        
        print(i .. "=" .. tostring(p));
        if (p.IsDirectory) then 
           print("is a dir.");
        else
            print("is NOT a dir."); 
        end
    end
    --local context = Context.New("{ROOT}", "{WILDCARD}");
    --print('Yet, the type of a Context is "' .. type(context) .. '".');
    --print("The Path is " .. path .. "? No?\n");
    
end