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
    
    attemptShortName = true,
    
    createClassGenerator = function (self, node, path)
        local reason = node.Member.ReasonCreated;
        local srcFile = tostring(reason.Source.FileName);        
        local filePath = path:NewPath(".cpp");
        if (filePath:IsFileOlderThan(srcFile)) then            
            local cg = ClassCppFileGenerator.new{node = node, path = filePath};
            return cg;
        else
            -- Skip if no new changes detected.
            return { parse = function() end };
        end
    end, 
    
}; -- end CppFileGenerator