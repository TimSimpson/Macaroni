require "Macaroni.Model.Cpp.Access";
require "Macaroni.IO.Path";
require "Macaroni.Model.Node";
require "Macaroni.Model.Context";
require "Macaroni.Model.Context";
require "Macaroni.Model.FileName"
require "Macaroni.Model.Cpp.Function";
require "Macaroni.Model.Member";
require "Macaroni.Model.Node";
require "Macaroni.Model.Cpp.VariableAssignment";
require "Macaroni.Model.Type";
require "Macaroni.Model.TypeArgument";
require "Macaroni.Model.TypeArgumentList";
require "Macaroni.Model.TypeList";
require "debug";
require "io";

-- Eventually put this next part in C++.
Macaroni.Model.TypeNames = 
{ 
    Class = "Class", 
    Constructor="Constructor", 
    Function="Function", 
    Namespace = "Namespace", 
    Primitive="Primitive",
    Typedef="Typedef",
    Variable="Variable"
};

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
            else
                path = '<' .. node.Node:GetPrettyFullName("/") .. '/_.h>';
            end            
        end        
        return ('#include ' .. path .. '\n');        
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

--function Generate(context, path)
--    print("I play no part in this cosmic dance. I prance, and prance, and prance...");
--end
--[[]]--
--print("CppCommon.lua is being loaded now.");