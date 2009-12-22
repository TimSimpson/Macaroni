require "Macaroni.Generator.Output.Path";
require "Macaroni.Model.Node";
require "Macaroni.Model.Context";

local Context = Macaroni.Model.Context;

function Generate(context, path)
    print "This illustrates a simple Generator.\n";
    print("Path type is " .. tostring(path) .. ".");  
end