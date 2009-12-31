require "Cpp/Common";
require "Cpp/ClassHFileGenerator";
require "Cpp/NamespaceHFileGenerator";
require "Cpp/NodeFileGenerator";

local Access = Macaroni.Model.Cpp.Access;
local Context = Macaroni.Model.Context;
local Node = Macaroni.Model.Node;
local TypeNames = Macaroni.Model.TypeNames;

HFileGenerator = {

    new = function()
        args = {}
        setmetatable(args, HFileGenerator);
        HFileGenerator.__index = function(t, k)
            local v = HFileGenerator[k];
            if (not v) then 
                v = NodeFileGenerator[k];
            end
            return v;
        end;

        return args;
    end,
    
    createClassGenerator = function (self, node, path)
        local filePath = path:NewPath(".h");
        local cg = ClassHFileGenerator.new{node = node, path = filePath};
        return cg;
    end,
    
    createNamespaceFileGenerator = function (self, node, path)
        local filePath = path:NewPath(".h");
        local ng = NamespaceHFileGenerator.new{node = node, path = filePath};
        return ng;
    end,
    
}; -- end HFileGenerator