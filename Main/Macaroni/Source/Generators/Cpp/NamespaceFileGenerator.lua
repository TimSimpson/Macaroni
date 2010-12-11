require "Cpp/Common";

local Access = Macaroni.Model.Cpp.Access;
local Context = Macaroni.Model.Context;
local Node = Macaroni.Model.Node;
local TypeNames = Macaroni.Model.TypeNames;

NamespaceFileGenerator = {
    new = function(args)
        if (args == nil) then 
            args = {} 
        end
        setmetatable(args, ClassHFileGenerator);        
        Util.linkToSubClass(ClassFileGenerator, ClassHFileGenerator);             
        return args;
    end,
    parse = function(self)
    end,
};