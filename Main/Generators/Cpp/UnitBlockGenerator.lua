require "Cpp/FileGenerator"

UnitBlockGenerator =
{
    isNested = false,
    tabs = 0,

    Write = function(args)
        assert(args.node ~= nil);
        assert(args.writer ~= nil);
        assert(args.node.Member ~= nil);
        local block = args.node.Member
        args.block = block
        setmetatable(args, UnitBlockGenerator);

        args:writeIncludes()
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
}

Util.linkToSubClass(FileGenerator, UnitBlockGenerator);
