require "Macaroni.Generator.Output.Path";
require "Macaroni.Model.Context";

local Context = Macaroni.Model.Context;

print "Hello, I am dumb.\n";

function Generate(context, path)
    print "Generating something, I guess.\n";
    print("Path type is " .. tostring(path) .. ".");
    
    local paths = path.Paths;
    for i = 0, #paths do
        print(i .. "=" .. tostring(v) .. "\n");
    end
    --local context = Context.New("{ROOT}", "{WILDCARD}");
    --print('Yet, the type of a Context is "' .. type(context) .. '".');
    --print("The Path is " .. path .. "? No?\n");
    
end