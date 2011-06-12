require "Macaroni.Model.Cpp.Access";
require "Cpp/Common";
require "Cpp/ClassFileGenerator";
require "Macaroni.Model.FileName";
require "Macaroni.Model.Reason";
FUTURE = true
if FUTURE then
	require "Macaroni.Model.Cpp.ClassParent";
	require "Macaroni.Model.Cpp.ClassParentList";
end
require "Macaroni.Model.Source";


local Access = Macaroni.Model.Cpp.Access;
local Context = Macaroni.Model.Context;
local Node = Macaroni.Model.Node;
local TypeNames = Macaroni.Model.TypeNames;

ClassHFileGenerator = {

    isNested = false,
    node = nil,
    tabs = 0,
    writer = nil,
    
    new = function(args)          
        assert(args.node ~= nil);
        assert(args.targetLibrary ~= nil);
        if (args.path == nil) then
            assert(args.writer ~= nil);
        else
            --TODO: Somehow calling C++ NewFileWriter() method currently results in entire Lua call from C++ ending.
            --local writer, errorMsg, errorNumber = io.open(args.path.AbsolutePath, 'w+'); --args.path:NewFileWriter(); 
            --if (writer == nil) then
            --    error(tostring(errorNumber) .. " " .. errorMsg, 2);                
            --end
            args.writer = args.path:CreateFile();--writer;
        end
        
        setmetatable(args, ClassHFileGenerator);
        args.libDecl = LibraryDecl(args.targetLibrary);
        return args;
    end,
    
    
    addTabs = function(self, tabCount)
        self.tabs = self.tabs + tabCount;
    end,
    
    classBegin = function(self)
        self:write("class ")
        if self.libDecl then
			self:write(self.libDecl .. " ");
        end
        self:write(self.node.Name .. "\n");
        if FUTURE then
			parents = self.node.Member.Parents
			if #parents > 0 then
				self:classParents(parents)
			end        
		end
        self:write("{\n");
        self:addTabs(1);
    end,
    
    classParents = function(self, parents)
		self:write(": ");
		for i=1, #parents do
			parent = parents[i]
			if i > 1 then
				self:write(",\n  ");
			end
			access = parent.Access
			self:write(access.CppName .. " ");
			if parent.IsVirtual == true then
				self:write("virtual ");
			end
			self:writeType(parent.Parent);
		end
		self:write("\n");		
    end,
    
    classBody = function(self)
        self:classBegin();
        self:classFriends();
        --[[
        self:write("public:\n");
        self:iterateMembers(self.node.Children, "Access_Public");
        self:write("protected:\n");
        self:iterateMembers(self.node.Children, "Access_Protected");
        self:write("private:\n");
        self:iterateMembers(self.node.Children, "Access_Private");
        ]]--
        self:iterateMembers(self.node.Children);
        self:classEnd();
    end,
    
    classEnd = function(self)        
        self:write("}; // End of class " .. self.node.Name .. "\n");
        self:addTabs(-1);
    end,
    
    classPublicGlobals = function(self)  
        assert(self.node.Member ~= nil 
               and 
               self.node.Member.TypeName == TypeNames.Class);
        local globals = self.node.Member.GlobalNodes;    
        self:write("/* Public Global Methods */\n");          
		self:iterateMembers(globals, Access.Public); 		
    end,
    
    classFriends = function(self)
        assert(self.node.Member ~= nil 
               and 
               self.node.Member.TypeName == TypeNames.Class);
        local friends = self.node.Member.FriendNodes;
        for i=1, #friends do
            friend = friends[i];
            if (friend.Member ~= nil 
                and friend.Member.TypeName == TypeNames.FunctionOverload) then
                self:write("friend ");
                self:writeFunctionOverloadDefinition(friend, false); -- ); --, true);
                self:write(";\n");
            elseif (friend.Member ~= nil 
				     and friend.Member.TypeName == TypeNames.Function) then
				self:write([[
/* Putting friend on a free standing node which resolves to a function means
Macaroni will apply the 'friend' status to all function overloads.  To only 
apply it to specific overloads, use the "~friend" keyword on the definitions
of those functions.  If this isn't possible, resort to a ~block. :( */]] .. '\n');
				for i = 1, #friend.Children do   
					self:write("friend "); 	
					self:writeFunctionOverloadDefinition(friend.Children[i], false);	
    				self:write(";\n");
    			end
			else
                self:write("friend " .. friend.FullName .. ";\n");
            end
        end 
    end,
    
    getGuardName = function(self)
        local guardName = "MACARONI_COMPILE_GUARD_" .. self.node:GetPrettyFullName("_") .. "_H";
        return guardName;
    end,
    
    getNodeAlias = function(self, node)
        return node.FullName;
    end,       
    
    includeStatements = function(self)            
        local section = DependencySection.new();
        section:add(self.node);
        section:eraseDuplicates();
        for i = 1, #section.list do
            local s = section.list[i];            
            if (s.heavy == false) then
                self:write(NodeInfoList[s.node].lightDef);
            else
                self:write(NodeInfoList[s.node].heavyDef);
            end
        end       
        
        --[[local statements = IncludeFiles.getHFileIncludeStatementsForNode(self.node);
        self.writer:write("/* ~ Includes ~ */\n");
        for i = 1, #statements do
            self.writer:write(statements[i]);
        end ]]--                  
    end,  
    
    -- Entry function.
    parse = function(self)        
        check(self ~= nil, "Instance method called without self.");
        check(self.writer ~= nil, "Instance writer missing.");
        if (not self.isNested) then  
            self:includeGuardHeader();
            self:write('\n');
        end
        
        local reason = self.node.Member.ReasonCreated;
        local src = reason.Source;
                
        self:write("// This class was originally defined in " .. tostring(src.FileName) .. "\n");
        
        if BoostConfigIsAvailable(self.targetLibrary) then
			self:write("\n#include <" 
			           .. LibraryConfigFile(self.targetLibrary) 
			           .. ">\n\n");
        end
        
        self:write("// Forward declaration necessary if this depends on anything which also depend on this.\n");
        if (not self.isNested) then 
            self:namespaceBegin(self.node.Node);
        end
        self:write("class " .. self.node.Name .. ";\n");
        if (not self.isNested) then
            self:namespaceEnd(self.node.Node);
        end
        self:write("\n");
        
        if (not self.isNested) then  
            self:includeStatements();            
            self:write('\n');            
            self:namespaceBegin(self.node.Node);
            self:write('\n');
        end
        
        self:write('\n');
        self:classPublicGlobals();
        self:write('\n');
        self:classBody();
        if (not self.isNested) then
            self:write('\n');
            self:namespaceEnd(self.node.Node);
            self:write('\n');
            self:includeGuardFooter();
        end
    end,    
    
    ["parse" .. TypeNames.Block] = function(self, node)    
        local block = node.Member;            
        if (block.Id == "h") then
            self:write(block.Code);
        end
    end,
    
    ["parse" .. TypeNames.Constructor] = function(self, node)
    	for i = 1, #(node.Children) do
    		local overloadNode = node.Children[i];
    		self:parseConstructorOverload(overloadNode);
		end       
    end,
    
    ["parse" .. TypeNames.ConstructorOverload] = function(self, node)
        self:writeTabs();
        self:writeAccess(node.Member.Access);
        if (node.Member.Inline) then
            self:write("inline ");
        end
        self:write(self.node.Name .. "(");
        self:writeArgumentList(node);
        self:write(")");        
        if (not node.Member.Inline) then
            self:write(";\n");
        else
            self:writeConstructorAssignments(node.Member.Assignments);
            self:write("\n");
            self:writeTabs();
            self:write("{\n");
            self:addTabs(1);
            
            self:writeTabs();
            self:write(node.Member.CodeBlock .. "\n");
            
            self:addTabs(-1);        
            self:writeTabs();
            self:write("}\n");
        end       
    end,
    
    ["parse" .. TypeNames.Destructor] = function(self, node)
		check(#node.Children == 1, "Destructor must have one child.");
		local overload = node.Children[1];
        self:writeTabs();
        self:writeAccess(overload.Member.Access);
        if (overload.Member.Inline) then
            self:write("inline ");
        end
        self:write('~' .. self.node.Name .. "(");
        self:writeArgumentList(overload);
        self:write(")");
        if (not overload.Member.Inline) then
            self:write(";\n");
        else
            self:write("\n");
            self:writeTabs();
            self:write("{\n");
            self:addTabs(1);
            
            self:writeTabs();
            self:write(overload.Member.CodeBlock .. "\n");
            
            self:addTabs(-1);        
            self:writeTabs();
            self:write("}\n");
        end        
    end,
    
    ["parse" .. TypeNames.Function] = function(self, node)
    	for i = 1, #(node.Children) do
    		local overloadNode = node.Children[i];
    		self:parseFunctionOverload(overloadNode);
		end
    end,    
    
    ["parse".. TypeNames.FunctionOverload] = function(self, node)
		local ownedByClass = (node.Node.Node == self.node)		
    	if (ownedByClass) then
    		if not self.internalDef then
    			if (not node.Member.Access.VisibleInHeader) then
    				return
    			end
    		end
            self:writeAccess(node.Member.Access);
        end                
        self:writeFunctionOverloadDefinition(node, ownedByClass);
        if (not node.Member.Inline) then
            self:write(";\n");
        else
            self:writeTabs();
            self:write("{\n");
            self:addTabs(1);
            
            self:writeTabs();
            self:write(node.Member.CodeBlock .. "\n");
            
            self:addTabs(-1);        
            self:writeTabs();
            self:write("}\n");
        end	
	end,
    
    parseMember = function(self, node)
        local m = node.Member;
        if (m == nil) then
            self:writeTabs();
            self:write("// No member info- " .. node.Name .. "\n");
        end
        local typeName = m.TypeName;
        local handlerFunc = nil;        
        if (typeName == TypeNames.Class) then
            -- Pass the new generator the same writer as this class.
            ClassHFileGenerator.New({isNested = true, node = node, 
                                     targetLibrary=self.targetLibrary, 
                                     writer = self.writer});
            handlerFunc = self.parseClass;
        else
            handlerFunc = self["parse" .. typeName];
        end
        
        if (handlerFunc ~= nil) then                       
            handlerFunc(self, node);
        else
            self:writeTabs();
            self:write("//     ~ Have no way to handle node " .. node.Name .. " with Member type " .. typeName .. ".\n");
        end 
    end,
    
    ["parse" .. TypeNames.Namespace] = function(self)
        self:writeTabs();
        self:write("//TODO: Create parseNamespace functionality for Namespaces within classes.");
    end,
    
    ["parse" .. TypeNames.Variable] = function(self, node)
        if (node.Node == self.node) then
            self:writeAccess(node.Member.Access);
        end
        self:writeVariableDefinition(node);
        --[[self:writeTabs();
        local variable = node.Member;
        self:writeType(variable.Type);
        self:write(node.Name .. ";\n");]]--
    end,
    
    writeAccess = function(self, access)				
		self:write(access.CppKeyword .. ": ");		
    end,
};

Util.linkToSubClass(FileGenerator, ClassHFileGenerator);             