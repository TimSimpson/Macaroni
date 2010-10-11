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
		log("HcreateClassGenerator 1");
        local reason = node.Member.ReasonCreated;
        local srcFile = tostring(reason.Source.FileName);        
        log("HcreateClassGenerator 1.5");
        local filePath = path:NewPath(".h");
        log("HcreateClassGenerator 2");
        log("Htype of filePath is " .. type(filePath) .. ".");
        log("HsrcFile is " .. srcFile .. ".");
        if (filePath:IsFileOlderThan(srcFile)) then   
			log("HcreateClassGenerator 2.2");         
            local cg = ClassHFileGenerator.new{node = node, path = filePath};
            return cg;
        else
			log("HcreateClassGenerator 2.5");
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