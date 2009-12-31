require "Cpp/Common";

local Access = Macaroni.Model.Cpp.Access;
local Context = Macaroni.Model.Context;
local Node = Macaroni.Model.Node;
local TypeNames = Macaroni.Model.TypeNames;

NodeFileGenerator = {
    iterateNodes = function (self, nodeChildren, path)
        for i = 1, #(nodeChildren) do
            n = nodeChildren[i];
            self:parseNode(n, path);
        end
    end,
        
    createNamespaceFileGenerator = function (self, node, path)
        return nil;
    end,

    parseClass = function (self, node, path)
        assert(node.Member.TypeName == TypeNames.Class);    
        local cg = self:createClassGenerator(node, path);
        cg:parse();
    end,
    
    parseNamespace = function (self, node, path)
        if (node.Member == nil or node.Member.TypeName ~= TypeNames.Namespace) then
            error("node argument must be namespace.", 2);
        end
        path:CreateDirectory();
        local ng = self:createNamespaceFileGenerator(node, path);
        if (ng ~= nil) then
            ng:parse();
        end
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
    end,
};