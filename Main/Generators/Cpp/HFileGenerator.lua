require "Cpp/Common";
require "Cpp/ClassHFileGenerator";
require "Cpp/LibraryConfigGenerator";
require "Cpp/NamespaceHFileGenerator";
require "Cpp/NodeFileGenerator";
require "Cpp/TypedefFileGenerator";

require "Macaroni.Model.FileName";
require "Macaroni.Model.Reason";
require "Macaroni.Model.Source";

local Access = Macaroni.Model.Cpp.Access;
local Context = Macaroni.Model.Context;
local Node = Macaroni.Model.Node;
local TypeNames = Macaroni.Model.TypeNames;

HFileGenerator = {

    new = function(library)
		if (library == nil) then
			error("No library argument given.");
		end
        args = {}
        setmetatable(args, HFileGenerator);
        args.targetLibrary = library;
        HFileGenerator.__index = function(t, k)
            local v = HFileGenerator[k];
            if (not v) then 
                v = NodeFileGenerator[k];
            end
            return v;
        end;
		args.libDecl = LibraryDecl(args.targetLibrary);
        return args;
    end,

    attemptShortName = false,
    
    createClassGenerator = function (self, node, path)
		log:Write("HcreateClassGenerator 1");
        local reason = node.Member.ReasonCreated;
        local srcFile = tostring(reason.Source.FileName);        
        log:Write("HcreateClassGenerator 1.5");
        local filePath = path:NewPath(".h");
        log:Write("HcreateClassGenerator 2");
        log:Write("Htype of filePath is " .. type(filePath) .. ".");
        log:Write("HsrcFile is " .. srcFile .. ".");
        if (filePath:IsFileOlderThan(srcFile)) then   
			log:Write("HcreateClassGenerator 2.2");         
            local cg = ClassHFileGenerator.new{node = node, 
											   targetLibrary=self.targetLibrary, 
											   path = filePath};
            return cg;
        else
			log:Write("HcreateClassGenerator 2.5");
            -- Skip if no new changes detected.
            return { parse = function() end };
        end                
    end,
    
    
    createNamespaceFileGenerator = function (self, node, path)
        local filePath = path:NewPath("/_.h");
        local ng = NamespaceHFileGenerator.new{
			node = node, 
			path = filePath, 
			targetLibrary = self.targetLibrary
			};
        return ng;
    end,
    
    createTypedefFileGenerator = function (self, node, path)
        local filePath = path:NewPath(".h");
        local tg = TypedefFileGenerator.new{
			node = node, 
			path = filePath, 
			targetLibrary = self.targetLibrary
			};
        return tg;
    end,
        
    
}; -- end HFileGenerator