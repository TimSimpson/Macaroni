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

	memberIsInTargetLibrary = function(self, member)
		if (member.Library ~= self.targetLibrary) then			
			return false;
        end
        return true;
	end,
	
    parseClass = function (self, node, path)
        assert(node.Member.TypeName == TypeNames.Class);  
        log:Write("Time to parse class.");  
        local cg = self:createClassGenerator(node, path);
        log:Write("PFSF!");
        cg:parse();
        log:Write("Oh, that was a sweet parse!");
    end,
    
    parseNamespace = function (self, node, path)
        if (node.Member == nil or node.Member.TypeName ~= TypeNames.Namespace) then
            error("node argument must be namespace.", 2);
        end
        path:CreateDirectory();
        local ng = self:createNamespaceFileGenerator(node, path);
        if (ng ~= nil and ng:hasEntriesInLibrary(self.targetLibrary)) then
            ng:parse();
        end
        self:iterateNodes(node.Children, path);
    end,
    
    parseNode = function (self, node, path)
        log:Write("~~~ " .. node.FullName);
        if (not NodeHelper.worthIteration(node)) then
            log:Write(" Skipped.\n");
            return;
        end
        
        local m = node.Member;
        if (m == nil) then
            return;
        end        
        log:Write(" Same library.  Generation time!");
        local typeName = m.TypeName;
        
        -- Don't generate Nodes from other libraries which are in this Context.
        if (typeName ~= TypeNames.Namespace 
			and not self:memberIsInTargetLibrary(m)) then
			log:Write(" Different library, not generating.\n");
			return;
        end
        
        log:Write("       " .. typeName);
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
            log:Write("     ~ Skipping");
        end 
        log:Write(" ^-- Conclude parse.");
    end,
};