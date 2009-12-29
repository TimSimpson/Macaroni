require "Cpp/Common";
require "Cpp/ClassCppFileGenerator";

local Access = Macaroni.Model.Cpp.Access;
local Context = Macaroni.Model.Context;
local Node = Macaroni.Model.Node;
local TypeNames = Macaroni.Model.TypeNames;

CppFileGenerator = {
    
    iterateNodes = function (self, nodeChildren, path)
        for i = 1, #(nodeChildren) do
            n = nodeChildren[i];
            self:parseNode(n, path);
        end
    end,

    new = function(self)
        self = self or {}
        setmetatable(self, CppFileGenerator);
        CppFileGenerator.__index = CppFileGenerator;
        return self;
    end,
    
    parseClass = function (self, node, path)
        assert(node.Member.TypeName == TypeNames.Class);    
        local filePath = path:NewPath(".cpp");
        local cg = ClassCppFileGenerator.new{node = node, path = filePath};
        cg:parse();
    end,

    parseNamespace = function (self, node, path)
        if (node.Member == nil or node.Member.TypeName ~= TypeNames.Namespace) then
            error("node argument must be namespace.", 2);
        end
        path:CreateDirectory();
        self:iterateNodes(node.Children, path);
    end,

    parseNode = function (self, node, path)
        print("~~~ " .. node.FullName);
        if (not NodeHelper.worthIteration(node)) then
            print(" Skipped.\n");
            return;
        end
        
        local m = node.Member;
        if (m == nil) then
            return;
        end
        local typeName = m.TypeName;
        print("       " .. typeName);
        local newPath = path:NewPath("/" .. node.Name);
        --if (newPath.IsDirectory) then
        --    newPath.CreateDirectory();
        --end
        
        local handlerFunc = nil;
        if (typeName == TypeNames.Namespace) then
            handlerFunc = self.parseNamespace;
        elseif (typeName == TypeNames.Class) then
            handlerFunc = self.parseClass;
        end
        
        if (handlerFunc ~= nil) then
            handlerFunc(self, node, newPath);
        else
            print("     ~ Skipping");
        end 
    end

}; -- end CppFileGenerator