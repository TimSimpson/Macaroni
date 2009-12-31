require "Cpp/Common";
require "Cpp/ClassCppFileGenerator";
require "Cpp/NodeFileGenerator";

local Access = Macaroni.Model.Cpp.Access;
local Context = Macaroni.Model.Context;
local Node = Macaroni.Model.Node;
local TypeNames = Macaroni.Model.TypeNames;

CppFileGenerator = {

    new = function(self)
        self = self or {}
        setmetatable(self, CppFileGenerator);
        CppFileGenerator.__index = function(t, k)
            local v = CppFileGenerator[k];
            if (not v) then 
                v = NodeFileGenerator[k];
            end
            return v;
        end;
        return self;
    end,
    
    createClassGenerator = function (self, node, path)
        local filePath = path:NewPath(".cpp");
        local cg = ClassCppFileGenerator.new{node = node, path = filePath};
        return cg;
    end, 
    
}; -- end CppFileGenerator