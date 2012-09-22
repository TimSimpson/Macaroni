require "Cpp/FileGenerator"
require "Cpp/NodeInfo"

UnitBlockGenerator =
{
    isNested = false,
    tabs = 0,

    usingStatements = function(self)
        local imports = self.block.ImportedNodes;
        for i = 1, #imports do
            local import = imports[i];
            self:writeUsing(import);
        end
    end,

    Write = function(args)
        assert(args.node ~= nil);
        assert(args.writer ~= nil);
        assert(args.node.Member ~= nil);
        local block = args.node.Member
        args.block = block
        setmetatable(args, UnitBlockGenerator);

        args:writeIncludes()
        args:usingStatements()
        args:writeBlockCodeBlock(args.node.Member)
    end,

    writeIncludes = function(self)
        local imports = self.block.ImportedNodes;
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
