require "Cpp/Common";
require "Cpp/ClassHFileGenerator";
require "Macaroni.Model.Library";
require "Cpp/NamespaceHFileGenerator";
require "Cpp/NodeFileGenerator";
require "Cpp/NodeInfo";

local Access = Macaroni.Model.Cpp.Access;
local Context = Macaroni.Model.Context;
local Node = Macaroni.Model.Node;
local TypeNames = Macaroni.Model.TypeNames;

MhFileGenerator = {

    new = function(path)
        args = {}
        setmetatable(args, MhFileGenerator);
        MhFileGenerator.__index = function(t, k)
            local v = MhFileGenerator[k];
            if (not v) then 
                v = FileGenerator[k];
            end
            return v;
        end;

        local filePath = path:NewPath("/Interface.mh");
        --TODO: Somehow calling C++ NewFileWriter() method currently results in entire Lua call from C++ ending.
        local writer, errorMsg, errorNumber = io.open(filePath.AbsolutePath, 'w+'); --args.path:NewFileWriter(); 
        if (writer == nil) then
            error(tostring(errorNumber) .. " " .. errorMsg, 2);                
        end
        args.writer = writer;
        args.tabs = 0;
        
        return args;
    end,
    
    
    iterateNodes = function (self, nodeChildren)
        for i = 1, #(nodeChildren) do
            n = nodeChildren[i];
            self:parseNode(n);
        end
    end,

    parseClass = function(self, node)
        if (node.Member.Library == targetLibrary) then
            local info = NodeInfoList[node];
            self:writeAfterTabs('class ' .. node.Name .. ' {\n');
            self:addTabs(1);
            self:writeAfterTabs('~hfile=' .. info.headerFile .. '\n');
            self:iterateNodes(node.Children);
            self:addTabs(-1);
            self:writeAfterTabs('};\n');
            -- Find the hFile
        end
    end,
    
    parseNamespace = function(self, node)
        if (node.Member.Library == targetLibrary) then
            local info = NodeInfoList[node];
            self:writeAfterTabs("namespace " .. node.Name .. ' {\n');
            self:addTabs(1);        
            -- self:writeAfterTabs('~hfile=' .. info.headerFile .. ';\n');        
            self:iterateNodes(node.Children);
            self:addTabs(-1);
            self:writeAfterTabs("}\n");    
        end
    end,
    
    parseNode = function(self, node) 
        if (node.Member == nil) then
            self:writeAfterTabs("// Skipping " .. node.Name .. '\n');
        else
            self:parseNodeMember(node);
        end        
    end, 
    
    parseNodeMember = function(self, node)
        local member = node.Member;
        if (member.TypeName == TypeNames.Class) then
            self:parseClass(node);
        elseif (member.TypeName == TypeNames.Namespace) then
            self:parseNamespace(node);   
        elseif (member.TypeName == TypeNames.Typedef) then
            self:parseTypedef(node);   
        else
            self:writeAfterTabs("//WARNING: Do not know how to handle " .. node.Name .. " with Member type " .. member.TypeName .. ".\n");
        end
    end,
    
    parseTypedef = function(self, node)
        local info = NodeInfoList[node];
        self:writeAfterTabs("class " .. node.Name .. ' { ' .. 
                            '~hfile=' .. info.headerFile .. '; }\n');
    end,
}; -- end HFileGenerator

Util.linkToSubClass(FileGenerator, MhFileGenerator);