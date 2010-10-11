require "Macaroni.Model.Cpp.Access";
require "Macaroni.IO.GeneratedFileWriter";
require "Macaroni.IO.Path";
require "Macaroni.Model.Node";
require "Macaroni.Model.Context";
require "Macaroni.Model.Context";
require "Macaroni.Model.FileName"
--require "Macaroni.Model.Cpp.Function";
require "Macaroni.Model.Member";
require "Macaroni.Model.Node";
require "Macaroni.Model.Type";
require "Macaroni.Model.TypeArgument";
require "Macaroni.Model.TypeArgumentList";
require "Macaroni.Model.TypeList";
require "Macaroni.Model.Cpp.VariableAssignment";
require "debug";
require "io";

-- Eventually put this next part in C++.
Macaroni.Model.TypeNames = 
{ 
    Block = "Block",
    Class = "Class", 
    Constructor="Constructor", 
    Destructor="Destructor", 
    Function="Function", 
    Namespace = "Namespace", 
    Primitive="Primitive",
    Typedef="Typedef",
    Variable="Variable"
};

-- Called to see if node is in that weird underscore header
function nodeDefinedInNamespaceHeader(node)
	local m = node.Member;
	if (m == null) then
		return false;
	end
	local t = m.TypeName;
	return t == Macaroni.Model.TypeNames.Function or 
		   t == Macaroni.Model.TypeNames.Typedef or
		   t == Macaroni.Model.TypeNames.Variable;
end

-- Identical to "assert" but points the error at the calling method.
function check(condition, errorMsg) 
    if (not condition) then
        error(errorMsg, 3);
    end
end

Util = {
    linkToSubClass = function(superClass, subClass)
        check(superClass ~= nil, "Argument 1 (superClass) must not be nil.");
        check(subClass ~= nil, "Argument 2 (subClass) must not be nil.");
        subClass.__index = function(t, k)
            local v = subClass[k];
            if (not v) then 
                v = superClass[k];
            end
            return v;
        end;
    end,
};

NodeHelper = {
    --[[createNamespaceOpener =  function(namespaceNode)
        check(namespaceNode ~= nil, "namespaceNode cannot be nil.");
        local fs = namespaceNode.FullName;
        local names = Macaroni.Model.Node.SplitComplexName(fs);
        local rtn = "";
        for i = 1, #names do
            rtn = rtn .. "namespace " .. names[i] .. " { ";
        end
        rtn = rtn .. "\n";
        return rtn;
    end,    
    
    createNamespaceCloser = function(namespaceNode)
        check(namespaceNode ~= nil, "namespaceNode cannot be nil.");
        local names = Macaroni.Model.Node.SplitComplexName(namespaceNode.FullName);
        local rtn = "";
        for i = 1, #names do
            rtn = rtn .. "} ";
        end
        rtn = rtn .. "// End namespace ";
        rtn = rtn .. "\n";
        return rtn;
    end,]]--
    
    worthIteration = function(node) 
        if (node == nil) then
            error("Nil not allowed for node argument.", 2);
        end
        if (node.HFilePath ~= nil) then
            -- The HFile means Macaroni isn't needed to generate this.
            return false;
        end 
        if (node.Member ~= nil) then
            if (node.Member.TypeName ~= Macaroni.Model.TypeNames.Namespace) then
                return true;
            end
        end
        -- If Node is a container, check if any of its kids are interesting.
        for i = 1, #(node.Children) do
            if (NodeHelper.worthIteration(node.Children[i])) then
                return true;
            end
        end
        return false;
    end
}

Common = {
    
};

IncludeFiles = {
    -- Given any Node, will create and return the correct include statement.
    createStatementForNode = function(node) 		    
        if (node.Member ~= nil and node.Member.TypeName == Macaroni.Model.TypeNames.Primitive) then
            return;
        end
        local path = nil;
        if (node.HFilePath ~= nil) then 
            path = tostring(node.HFilePath);
        else
            if (node.AdoptedHome ~= nil) then
                return createStatementForNode(node.AdoptedHome);
            end
            if (node.Member ~= nil and node.Member.TypeName == Macaroni.Model.TypeNames.Class) then
                path = '<' .. node:GetPrettyFullName("/") .. '.h>'; 
            elseif (nodeDefinedInNamespaceHeader(node)) then				
                path = '<' .. node.Node:GetPrettyFullName("/") .. '/_.h>';
            else
				path = nil;
            end            
        end        
        if (path ~= nil) then
			return ('#include ' .. path .. '\n');        
		end
    end,
        
    createStatementForNodeAndPutInTable = function(node, rtnTable)
        local include = IncludeFiles.createStatementForNode(node);
        if (include ~= nil) then
            rtnTable[include] = true;            
        end
    end,
    
    -- Given a Node, finds needed include statmenets, but doesn't make one for node itself.
    -- So for example, if you defined a variable "std::string blah" this would create
    -- the includes for <string>, but not for "blah." If you pass this a class,
    -- for instance, nothing is generated.
    getStatementsForNodeDependencies = function(node, rtnTable)
        if (node.Member ~= nil) then
            IncludeFiles.getStatementsForNodeMember(node.Member, rtnTable);
        end        
    end,
    
    -- Given a function argument list, finds and puts into table any needed include statements
    getStatementsForArgumentList = function(argList, rtnTable)
         for i = 1, #(argList) do
            IncludeFiles.getStatementsForNodeMember(argList[i], rtnTable);   
         end
    end,
    
    -- Given array of nodes, finds any needed include statements and places them into table.
    getStatementsForNodeArray = function(nodeList, rtnTable)
         for i = 1, #(nodeList) do
            IncludeFiles.getStatementsForNodeDependencies(nodeList[i], rtnTable);   
         end
    end,
    
    -- Given a member, places any necessary include statements into the provided table.
    getStatementsForNodeMember = function(member, rtnTable)      
        if (rtnTable == nil) then
            error("rtnTable may not be nil!", 2);
        end  
        if (member.TypeName == Macaroni.Model.TypeNames.Variable) then
            IncludeFiles.getStatementsForType(member.Type, rtnTable);
            --local include = IncludeFiles.createStatementForNode(member.Type.Node);
            --if (include ~= nil) then
            --    rtnTable[include] = true;            
            --end
        end
        if (member.TypeName == Macaroni.Model.TypeNames.Function) then                            
            IncludeFiles.getStatementsForType(member.ReturnType, rtnTable);
            --local include = IncludeFiles.createStatementForNode(member.ReturnType.Node);
            --if (include ~= nil) then
            --    rtnTable[include] = true;
            --end            
        end
        if (member.TypeName == Macaroni.Model.TypeNames.Function or
            member.TypeName == Macaroni.Model.TypeNames.Constructor) then      
            IncludeFiles.getStatementsForArgumentList(member.Arguments, rtnTable);
        end
        if (member.TypeName == Macaroni.Model.TypeNames.Typedef) then
            IncludeFiles.getStatementsForType(member.Type, rtnTable);
        end
    end,
    
    -- Given the type, finds all needed include statements and places into table.
    getStatementsForType = function(type, rtnTable)
        check(type ~= nil, "Type cannot be nil.");
        check(rtnTable ~= nil, "RtnTable can't be nil.");        
        IncludeFiles.createStatementForNodeAndPutInTable(type.Node, rtnTable);
        if (type.TypeArguments ~= nil) then
            for i = 1, #type.TypeArguments do
                local typeArg = type.TypeArguments[i];
                IncludeFiles.createStatementForNodeAndPutInTable(typeArg.Node, rtnTable);
                for j = 1, #typeArg.Arguments do
                    IncludeFiles.getStatementsForType(typeArg.Arguments[j], rtnTable);
                end
            end
        end
    end,
    
    -- Given a list of nodes returns Nodes which minor nodes depend on.
    getListOfStatementsForNodeList = function(nodeList)
        local map = {};
        IncludeFiles.getStatementsForNodeArray(nodeList, map);                        
        local rtn = {};
        for k, v in pairs(map) do            
            rtn[#rtn + 1] = k;
        end
        return rtn;
    end,
    
    -- Given a Node, returns a list of Nodes which will need to be included in 
    -- the header file.
    getHFileIncludeStatementsForNode = function (node)
        local member = node.Member;
        if (member ~= nil) then
            if (member.TypeName == Macaroni.Model.TypeNames.Class) then
                -- All fields (variables) must force an import.
                return IncludeFiles.getListOfStatementsForNodeList(node.Children);                
            end
        end 
        return {};
    end
}

TypeUtil = {
    new = function() 
        local self = {};
        setmetatable(self, { ["__index"] = TypeUtil } );
        --setmetatable(self, TypeUtil);
        return self;
    end,
    
    -- Returns a String with code defining the type.  If attemptShortName is
    -- true, it will not use the full name of the Type node.  If the type is
    -- complex, full names are used regardless.
    createTypeDefinition = function(self, type, attemptShortName) 
        check(self ~= nil, "Member method called without instance.");
        check(type ~= nil, 'Argument 2 "type" can not be null.');    
        
        local rtnStr = "";
        if (type == nil) then
            error("Type argument cannot be nil.", 2);           
        end
        if (type.Const) then
            rtnStr = rtnStr .. "const ";
        end
        local typeArguments = type.TypeArguments;
        if (self:forceShortName(type, attemptShortName)) then
            rtnStr = rtnStr .. type.Node.Name;
        else
            if (typeArguments == nil) then
                rtnStr = rtnStr .. type.Node.FullName;
            else
                local nodeList = self:createPathListFromNode(type.Node);            
                for i = 1, #nodeList do
                    local nodePart = nodeList[i];
                    local typeArg = self:searchTypeArgumentListForNode(typeArguments, nodePart);
                    rtnStr = rtnStr .. nodePart.Name;
                    if (typeArg ~= nil) then
                        rtnStr = rtnStr .. "<";                      
                        for i = 1, #typeArg.Arguments do                        
                            rtnStr = rtnStr .. self:createTypeDefinition(typeArg.Arguments[i]);
                        end
                        rtnStr = rtnStr .. ">";                     
                    end
                    if (i < #nodeList) then
                        rtnStr = rtnStr .. "::";
                    end
                end            
            end
        end        
        rtnStr = rtnStr .. ' ';
        if (type.Pointer) then
            rtnStr = rtnStr .. "* ";
        end
        if (type.Reference) then
            rtnStr = rtnStr .. "& ";
        end
        if (type.ConstPointer) then
            rtnStr = rtnStr .. "const ";
        end    
        
        return rtnStr;
    end,
    
    -- given node A::B::C, returns array { A, A::B, A::B::C }
    createPathListFromNode = function(self, node)
        check(node ~= nil, "Cannot iterate nil node!");
        local i = 0;
        local itr = node;
        while (not itr.IsRoot) do
            i = i + 1;
            itr = itr.Node;
        end        
        local rtn = {}
        local itr2 = node;
        while (not itr2.IsRoot) do
            rtn[i] = itr2;
            i = i - 1;
            itr2 = itr2.Node;
        end
        return rtn;        
    end,   
    
    -- True if the short name of the type's node should be forced.
    forceShortName = function(self, type, desireShortName) 
        if (type.Node.Member ~= nil  and type.Node.Member.TypeName == Macaroni.Model.TypeNames.Primitive) then
            return true;
        end
        if (desireShortName and 
            (type.TypeArguments == nil or #type.TypeArguments <= 0)) then
            return true;
        end
        return false;
    end,
    
     -- Searches the TypeArgumentList for a TypeArgument with the given node.
    searchTypeArgumentListForNode = function(self, typeArgList, node)
        check(typeArgList ~= nil, "TypeArgument list can't be nil.");
        check(node ~= nil, "Node can't be nil.");
        for i = 1, #typeArgList do
            local tArg = typeArgList[i];
            if (tArg.Node == node) then
                return tArg;
            end
        end
        return nil;
    end,   
        
}

--function Generate(context, path)
--    print("I play no part in this cosmic dance. I prance, and prance, and prance...");
--end
--[[]]--
--print("CppCommon.lua is being loaded now.");