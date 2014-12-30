require "Cpp/Common";
require "Cpp/ClassHFileGenerator";
require "Cpp/NamespaceHFileGenerator";
require "Cpp/NodeFileGenerator";
require "Cpp/NodeInfo";

local Access = require "Macaroni.Model.Cpp.Access";
local Context = require "Macaroni.Model.Context";
local Node = require "Macaroni.Model.Node";
local TypeNames = Macaroni.Model.TypeNames;

MhFileGenerator = {

    new = function(output, interfacePath, targetLibrary)
        args = {}
        setmetatable(args, MhFileGenerator);
        MhFileGenerator.__index = function(t, k)
            local v = MhFileGenerator[k];
            if (not v) then
                v = FileGenerator[k];
            end
            return v;
        end;

        local filePath = interfacePath;
        --TODO: Somehow calling C++ NewFileWriter() method currently results in entire Lua call from C++ ending.
        --local writer, errorMsg, errorNumber = io.open(filePath.AbsolutePath, 'w+'); --args.path:NewFileWriter();
        --if (writer == nil) then
        --    error(tostring(errorNumber) .. " " .. errorMsg, 2);
        --end
        --args.writer = writer;
        args.lines = {}
        args.output = output;
        args.writer = filePath:CreateFile();
        args.tabs = 0;
        args.targetLibrary = targetLibrary;
        return args;
    end,


    iterate = function (self)
        local nodeChildren = self.targetLibrary.Context.Root.Children
        self:iterateNodes(nodeChildren)
        table.sort(self.lines, function (a, b)
            return string.lower(a) < string.lower(b)
        end)
        self:writeAfterTabs("// Macaroni interface for "
                            .. tostring(self.targetLibrary) .. "\n\n");
        for i, v in ipairs(self.lines) do
            self:writeAfterTabs(v)
        end
    end,

    iterateNodes = function(self, nodeChildren)
        for i = 1, #(nodeChildren) do
            local node = nodeChildren[i];
            local element = node.Element
            if element ~= nil and element:OwnedBy(self.targetLibrary) then
                self:parseElement(node, element);
            end
            if #(node.Children) > 0 then
                self:iterateNodes(node.Children)
            end
        end
    end,

    parseClass = function(self, node, element)
        local info = NodeInfoList[node];
        self.lines[#self.lines + 1] = 'class ' .. node.FullName .. ' {\t'
                                .. '~hfile=' .. info.headerFile .. '\t};\n';
    end,

  --   parseNamespace = function(self, node)
		-- if (node.Element.Library == self.targetLibrary) then
  --           local info = NodeInfoList[node];
  --           self:writeAfterTabs("namespace " .. node.Name .. ' {\n');
  --           self:addTabs(1);
  --           -- self:writeAfterTabs('~hfile=' .. info.headerFile .. ';\n');
  --           self:iterateNodes(node.Children);
  --           self:addTabs(-1);
  --           self:writeAfterTabs("}\n");
  --       end
  --   end,

    parseElement = function(self, node, element)
        if (element.TypeName == TypeNames.Class) then
            self:parseClass(node, element);
        -- elseif (element.TypeName == TypeNames.Namespace) then
        --     self:parseNamespace(node);
        elseif (element.TypeName == TypeNames.Typedef) then
            self:parseTypedef(node, element);
        -- else
        --     self:writeAfterTabs("//WARNING: Do not know how to handle " .. node.Name .. " with Element type " .. element.TypeName .. ".\n");
        end
    end,

    parseTypedef = function(self, node, element)
        local info = NodeInfoList[node];
        self.lines[#self.lines + 1] = "class " .. node.FullName .. " {\t" ..
                                      "~hfile=" .. info.headerFile .. "\t};\n";
    end,
}; -- end HFileGenerator

Util.linkToSubClass(FileGenerator, MhFileGenerator);
