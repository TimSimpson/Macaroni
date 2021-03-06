require "Cpp/Common";
require "Cpp/ClassCppFileGenerator";
require "Cpp/NodeFileGenerator";

local Access = require "Macaroni.Model.Cpp.Access";
local Context = require "Macaroni.Model.Context";
local Node = require "Macaroni.Model.Node";
local TypeNames = Macaroni.Model.TypeNames;

CppFileGenerator = {

    new = function(library)
		if (library == nil) then
			error("No library argument given.");
		end
        self = {}
        setmetatable(self, CppFileGenerator);
        self.targetLibrary = library;
        CppFileGenerator.__index = function(t, k)
            local v = CppFileGenerator[k];
            if (not v) then
                v = NodeFileGenerator[k];
            end
            return v;
        end;
        self.libDecl = LibraryDecl(self.targetLibrary);
        return self;
    end,

    attemptShortName = true,

    createClassGenerator = function (self, node, path)
		log:Write('CPP File Gen createClassGenerator 1');
        local reason = node.Member.ReasonCreated;
        local srcFile = tostring(reason.Source.FileName);
        local filePath = path:NewPath(".cpp");
        if MACARONI_VERSION ~= "0.1.0.23" then
            node.Element.Owner:SetCppFile(filePath);
        end
        log:Write('CPP File Gen createClassGenerator 2');
        --if (filePath:IsFileOlderThan(srcFile)) then
            local cg = ClassCppFileGenerator.new{node = node,
				targetLibrary = self.targetLibrary, path = filePath};
            log:Write('CPP File Gen createClassGenerator 3');
            return cg;
        --else
        --    -- Skip if no new changes detected.'
        --    log:Write('File for ' .. tostring(node) ..' is up to date, skipping.');
        --    return { parse = function() end };
        --end
    end,

    parseTypedef = function (self, node, path)
		-- Do nothing
    end,

}; -- end CppFileGenerator
