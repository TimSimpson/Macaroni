require "Cpp/Common"
require "Cpp/FileGenerator"
require "Cpp/NodeInfo"

UnitBlockGenerator =
{
    isNested = false,
    tabs = 0,

    new = function(self)
        assert(self.writer ~= nil);
        assert(self.node.Element ~= nil);
        setmetatable(self, UnitBlockGenerator)

        self.block = self.node.Element
        assert(self.block.TypeName == "Block")
        return self
    end,

    usingStatements = function(self, block)
        local imports = block.ImportedNodes;
        for i = 1, #imports do
            local import = imports[i];
            self:writeUsing(import);
        end
    end,

    WriteTopBlocks = function(self)
        if self.block.Id == "top" then
            self:writeBlockCodeBlock(self.block)
        end
    end,

    WriteBottomBlocks = function(self)
        if self.block.Id == "bottom" then
            self:writeBlockCodeBlock(self.block)
        end
    end,

    WriteIncludeStatements = function(self)
        if (self.block.Id == "h"
            or self.block.Id == "h-predef"
            or self.block.Id == "h-postdef")
        then
            self:writeIncludes(self.block)
        end
    end,

    WriteImplementationIncludeStatements = function(self)
        if self.block.Id == "cpp" or self.block.Id == "cpp-include" then
            self:writeIncludes(self.block)
        end
        if self.block.Id == "cpp-include" then
            self:writeBlockCodeBlock(self.block)
        end
    end,

    WriteUsingStatements = function(self)
        if self.block.Id == "cpp" then
            self:usingStatements(self.block)
        end
    end,

    WritePreDefinitionBlocks = function(self)
        if self.block.Id == "h-predef" then
            self:writeBlockCodeBlock(self.block)
        end
    end,

    WriteHeaderDefinitions = function(self)
        if self.block.Id == "h" then
            self:writeBlockCodeBlock(self.block)
        end
    end,

    WritePostDefinitionBlocks = function(self)
        if self.block.Id == "h-postdef" then
            self:writeBlockCodeBlock(self.block)
        end
    end,

    WriteImplementation = function(self)
        if self.block.Id == "cpp" then
            self:writeBlockCodeBlock(self.block)
        end
    end,

    writeIncludes = function(self, block)
        local imports = block.ImportedNodes;
        for i = 1, #imports do
            local import = imports[i];
            if (import.Member ~= nil) then
                self:writeInclude(import);
            else
                self:write('// Skipping hollow imported node "' .. import.FullName .. '"' .. "   \n");
            end
        end
    end,

    writeInclude = function(self, import)
        local statement = IncludeFiles.createStatementForNode(import);
        if (statement ~= nil) then self:write(statement); end
    end,

    writeUsing = function(self, import)
        self:write(NodeInfoList[import].using);
    end,
}

Util.linkToSubClass(FileGenerator, UnitBlockGenerator);
