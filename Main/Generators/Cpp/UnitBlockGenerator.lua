require "Cpp/FileGenerator"
require "Cpp/NodeInfo"

UnitBlockGenerator =
{
    isNested = false,
    tabs = 0,

    usingStatements = function(self, block)
        local imports = block.ImportedNodes;
        for i = 1, #imports do
            local import = imports[i];
            self:writeUsing(import);
        end
    end,

    Write = function(args)
        assert(args.writer ~= nil);
        --assert(args.node.Member ~= nil);
        assert(args.unit ~= nil);
        assert(args.type ~= nil);

        --local block = args.node.Member
        --args.block = block
        setmetatable(args, UnitBlockGenerator);

        local elements = args.unit:CreateElementList()

        topBlocks = {}
        bottomBlocks = {}
        middleBlocks = {}

        for i = 1, #elements do
            local element = elements[i]
            local member = element.Node.Member
            if element.Node.TypeName == "Block" then
                if member.Id == "top" then
                    topBlocks[#topBlocks + 1] = member
                elseif member.Id == "bottom" then
                    bottomBlocks[#bottomBlocks + 1] = member
                elseif member.Id == args.type then -- cpp or h
                    middleBlocks[#middleBlocks + 1] = member
                end
            end
        end

        for i = 1, #topBlocks do
            args:writeBlockCodeBlock(topBlocks[i])
        end

        for i = 1, #middleBlocks do
            args:writeIncludes(middleBlocks[i])
            args:usingStatements(middleBlocks[i])
        end

        for i = 1, #middleBlocks do
            args:writeBlockCodeBlock(middleBlocks[i])
        end

        for i = 1, #bottomBlocks do
            args:writeBlockCodeBlock(bottomBlocks[i])
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
