require "Cpp/Common";
require "Cpp/FileGenerator";
require "Cpp/DependencySection";
require "Cpp/LibraryConfigGenerator";

local Access = require "Macaroni.Model.Cpp.Access";
local Context = require "Macaroni.Model.Context";
local Node = require "Macaroni.Model.Node";
local TypeNames = Macaroni.Model.TypeNames;


TypedefFileGenerator = {

    isNested = false,
    node = nil,
    tabs = 0,
    writer = nil,

    new = function(args)
        if (args == nil or args.node == nil or (args.path == nil and args.writer == nil)) then
            error('Expected a table to create new instance with fields "node" and "path" or "writer".', 2);
        end
        if (args.path == nil) then
            assert(args.writer ~= nil);
        else
            args.writer = args.path:CreateFile();
        end

        setmetatable(args, TypedefFileGenerator);

        args.minors = args:createMinorNodeList();

        args.libDecl = LibraryDecl(args.targetLibrary);

        return args;
    end,

    createMinorNodeList = function(self)
        return { self.node }
    end,

    debugOutDependencyGraph = function(self)
		if not CPP_GENERATE_VERBOSE then
			return;
		end
        self:write("/* ~ Debug Output of Dependency Graph ~ */\n");
        for i = 1, #self.minors do
            local m = self.minors[i];
            self:write("/* " .. m.FullName .. '\n');
            local info = NodeInfoList[m];
            self:write('     ld    : ' .. info._lightDef .. ' \n');
            self:write('     hd    : ' .. info._heavyDef .. ' \n');
            self:write('     using : ' .. info.using .. ' \n');
            local d = info.dependencies;
            self:write("     light dependencies : \n");
            for node in d:iterateLightDependencies() do
                self:write('                   ' .. node.FullName .. '\n');
            end
            self:write("\n");
            self:write("     HEAVY dependencies : \n");
            for node in d:iterateHeavyDependencies() do
                self:write('                   ' .. node.FullName .. '\n');
            end
            self:write("*/\n");
        end
    end,

    getGuardName = function(self)
        local guardName = "MACARONI_COMPILE_GUARD_" .. self.node:GetPrettyFullName("_") .. "___H";
        return guardName;
    end,

    nodeEligibleForParsing = function(self, node, targetLibrary)
		if (node == nil
			or node.Member == nil) then
			return false;
		end
		if (node.Member.Library ~= targetLibrary) then
			return false;
		end
		return self:memberEligibleForOutput(node.Member);
	end,

    hasEntriesInLibrary = function(self, targetLibrary)
		return true;
    end,

    includeStatements = function(self)
        local section = DependencySection.new();
        for i = 1, #self.minors do
			if CPP_GENERATE_VERBOSE then
				self:write("/* Adding minor " .. self.minors[i].FullName .. "*/\n");
			end
            section:add(self.minors[i]);
        end
        for i = 1, #section.list do
            local s = section.list[i];
            if (s.heavy == false) then
				self:writeVerbose("/* Light: " .. s.node.FullName .. "*/\n");
                self:write(NodeInfoList[s.node].useLightDef(self.targetLibrary));
                self:writeVerbose("/* END Light: " .. s.node.FullName .. "*/\n");
            else
				self:writeVerbose("/* Heavy: " .. s.node.FullName .. "*/\n");
                self:write(NodeInfoList[s.node].useHeavyDef(self.targetLibrary));
                self:writeVerbose("/* END Heavy: " .. s.node.FullName .. "*/\n");
            end
        end
    end,

    memberEligibleForOutput = function(self, member)
		local typeName = member.TypeName;
		local handlerFunc = nil;
        if (typeName == TypeNames.Typedef) then
            handlerFunc = self.parseTypedef;
        else
            handlerFunc = self["parse" .. typeName];
        end
        return handlerFunc ~= nil;
    end,

    parse = function(self)
        check(self ~= nil, "Instance method called without self.");
        check(self.writer ~= nil, "Instance writer missing.");
        self:includeGuardHeader();
        self:debugOutDependencyGraph();
        self:includeConfigFile();
        self:write('\n');
        self:includeStatements();
        self:write('\n');
        self:namespaceBegin(self.node.Node);
        self:write('\n');

        self:iterateMembers(self.minors);

        self:write('\n');
        self:namespaceEnd(self.node.Node);
        self:write('\n');
        self:includeGuardFooter();
    end,

    parseMember = function(self, node)
        local m = node.Member;
        if (m == nil) then
            self:writeTabs();
            self:write("// No member info- " .. node.Name .. "\n");
        end
        local typeName = m.TypeName;
        local handlerFunc = nil;
        if (typeName == TypeNames.Typedef) then
            handlerFunc = self.parseTypedef;
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

    ["parse" .. TypeNames.Typedef] = function(self, node)
        self:writeTabs();
        local typedef = node.Member;
        self:write("typedef ");
        self:writeType(typedef.Type);
        self:write(node.Name .. ";\n");
    end,

    shouldIncludeNode = function(self, node)
        if (node.Member ~= nil
                and node.Member.TypeName ~= TypeNames.Class
                and node.AdoptedHome == nil) then
            if (node.HFilePath == nil) then
				return true;
			end
            local attr = node.Annotations["Macaroni::Cpp::LightDef"];
            if (attr ~= nil) then
                return true;
            end
			local attr = node.Annotations["Macaroni::Cpp::UseLightDef"]
			if attr ~= nil and not attr.ValueAsBool then
				return true;
			end
        end
        return false;
    end,

};

Util.linkToSubClass(FileGenerator, TypedefFileGenerator);
