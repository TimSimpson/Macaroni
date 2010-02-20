require "Cpp/Common";
require "Cpp/ClassHFileGenerator";
require "Cpp/NamespaceHFileGenerator";
require "Cpp/NodeFileGenerator";

require "Macaroni.Model.FileName";
require "Macaroni.Model.Reason";
require "Macaroni.Model.Source";

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

    attemptShortName = false,
    
    createClassGenerator = function (self, node, path)
        local reason = node.Member.ReasonCreated;
        local srcFile = tostring(reason.Source.FileName);        
        local filePath = path:NewPath(".h");
        if (filePath:IsFileOlderThan(srcFile)) then            
            local cg = ClassHFileGenerator.new{node = node, path = filePath};
            return cg;
        else
            -- Skip if no new changes detected.
            return { parse = function() end };
        end        
    end,
    
    
    createNamespaceFileGenerator = function (self, node, path)
        local filePath = path:NewPath("/_.h");
        local ng = NamespaceHFileGenerator.new{node = node, path = filePath};
        return ng;
    end,
    
}; -- end HFileGenerator