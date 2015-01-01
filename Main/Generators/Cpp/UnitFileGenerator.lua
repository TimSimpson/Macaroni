require "Cpp/Common";
require "Plugin";
require "Cpp/ClassCppFileGenerator";
require "Cpp/ClassHFileGenerator";
local CppEnumFileGenerator = require "Cpp/EnumFileGenerator";
local FunctionGenerator = require "Cpp/FunctionFileGenerator";
require "Cpp/TypedefFileGenerator";
require "Cpp/UnitBlockGenerator";

local Access = require "Macaroni.Model.Cpp.Access";
local Context = require "Macaroni.Model.Context";
local EnumHFileGenerator = CppEnumFileGenerator.EnumHFileGenerator
local EnumCppFileGenerator = CppEnumFileGenerator.EnumCppFileGenerator
local FunctionFileGenerator = FunctionGenerator.FunctionFileGenerator
local Node = require "Macaroni.Model.Node";
local TypeNames = Macaroni.Model.TypeNames;

-- The older method of generating is to iterate by Node, and for each type
-- (class, function, etc) figure out whether or not to generate a file and
-- proceed from there.
-- That isn't quite flexible enough, so the new system is this. It iterates by
-- unit, building the header and cpp file for each one before proceeding.

FileWriters = {
    H = {
        Block = function(library, node, writer, unit)
            UnitBlockGenerator.Write{writer=writer, unit=unit, type="h"}
        end,

        Class = function(library, node, writer)
            local gen = ClassHFileGenerator.new{
                node = node, targetLibrary=library, writer=writer};
            gen:parse()
            writer:Write('\n'); -- avoid bunching up typedefs
        end,

        Enum = function(library, node, writer)
            local gen = EnumHFileGenerator.new{
                node=node, targetLibrary=library, writer=writer};
            gen:parse()
            writer:Write('\n');
        end,

        FunctionOverload = function(library, node, writer)
            local gen = FunctionFileGenerator.new{
                node = node,
                targetLibrary=library,
                writer=writer,
                insertIntoNamespaces=true,
                ownedByClass=false,
                classPrefix=nil,
            };
            gen:WriteIncludeStatements()  --TODO: Move this all to the same part!
            gen:WriteHeaderDefinition();
            writer:Write('\n');
        end,

        Typedef = function(library, node, writer)
            gen = TypedefFileGenerator.new{node=node, targetLibrary=library,
                                           writer=writer};
            gen:parse();
            writer:Write('\n'); -- avoid bunching up typedefs
        end,
    },
    Cpp = {
        Block = function(library, node, writer, unit)
            UnitBlockGenerator.Write{writer=writer, unit=unit, type="cpp"}
        end,

        Class = function(library, node, writer)
            gen = ClassCppFileGenerator.new{node = node, targetLibrary=library,
                                          writer=writer};
            gen:parse()
            writer:Write('\n'); -- avoid bunching up typedefs
        end,

        Enum = function(library, node, writer)
            gen = EnumCppFileGenerator.new{node=node, targetLibrary=library,
                                        writer=writer};
            gen:parse()
            writer:Write('\n');
        end,

        FunctionOverload = function(library, node, writer)
            local gen = FunctionFileGenerator.new{
                node = node,
                targetLibrary=library,
                writer=writer,
                insertIntoNamespaces=true,
                ownedByClass=false,
                classPrefix=nil,
            };
            gen:WriteIncludeStatements()  --TODO: Move this all to the same part!
                -- TODO: Don't call this- actually write the definition, which
                --       may do this. :)

            gen:WriteCppDefinition();
            writer:Write('\n');
        end,
    },
};


local CreateElementList = function(unit)
    -- Create an ordered list of elements so that the unit will be
    -- generated in the correct way, with any Nodes that depend on other
    -- Nodes in the unit being written later.
    local elements = unit:CreateElementList()
    local swappedNodes = {}
    for i = 1, #elements do
        ::elementCheck::
        local node = elements[i].Node
        info = NodeInfoList[node]
        -- Now, check if this node depends on Nodes which will be
        -- written to the file after this, and swap the two nodes if that's
        -- true.
        for k, dependencyNode in pairs(info.dependencies.heavy) do
            if dependencyNode == node then
                -- In other words, the node is it's own dependency (duh).
                -- Skip it.
                goto nextDependencyNodeContinue
            end
            for j = i, #elements do
                local node2 = elements[j].Node
                if dependencyNode == node2 then
                    -- Check if we already swapped these, raise an error
                    -- if we have.
                    for k2, v2 in pairs(swappedNodes) do
                        if v2 == elements[j] then
                            error("Can't determine how to order the nodes "
                                .. tostring(k2) .. " and "
                                .. tostring(v2) .. " in Unit "
                                .. tostring(unit) .. ".")
                        end
                    end
                    swappedNodes[elements[i]] = elements[j]
                    local swap = elements[i]
                    elements[i] = elements[j]
                    elements[j] = swap
                    goto elementCheck -- reiterate this index
                end
            end
            ::nextDependencyNodeContinue::
        end
    end

    return elements
end

local UnitGuardName = function(unit, fileType)
    return "MACARONI_UNIT_COMPILE_GUARD_" .. unit:GetShortCId()
           .. "_" .. fileType;
end

WriteUnitFileOpening = function(unit, writer, fileType)
    local guardName = UnitGuardName(unit, fileType)
    writer:WriteLine("#ifndef " .. guardName)
    writer:WriteLine("#define " .. guardName)
end

WriteUnitFileClosing = function(unit, writer, fileType)
    local guardName = UnitGuardName(unit, fileType)
    writer:WriteLine("#endif //" .. guardName)
end


LibraryTargetGenerator = {
    new = function(library)
        if (library == nil) then
            error("No library argument given.");
        end
        args = {}
        setmetatable(args, LibraryTargetGenerator);
        args.library = library;
        -- if path == nil then
        --     error("Argument #2, 'path', cannot be nil.")
        -- end
        -- args.rootPath = path;
        LibraryTargetGenerator.__index = function(t, k)
            local v = LibraryTargetGenerator[k];
            return v;
        end;
        return args;
    end,

    IterateUnits = function (self, library, rootPath)
        for unit in Plugin.IterateChildDependencies(library) do
            self:writeUnitFiles(unit, rootPath);
        end
    end,

    writeUnitFiles = function(self, unit, rootPath)
        if not unit.Generated then
            log:Write("Skipping unit " .. tostring(unit) .. ".")
            return
        end
        log:Write("Writing unit files for " .. tostring(unit) .. ".")

        args = {
            library = self.library,
            unit = unit,
            rootPath = rootPath,
            fileType = "H",
        };
        local h   = UnitFileGenerator.new(args);
        h:WriteFile();
        args.fileType = "Cpp"
        local cpp = UnitFileGenerator.new(args);
        cpp:WriteFile();
    end,
}


--TODO: CHANGE THIS in a major way to:
--  * Iterate the targets multiple times: first writing top blocks,
--    In H files:
--          library configs,
--          include statements,
--          "light defs",
--          impls,
--    then, in CPP files, also:
--          "internal header" version of above
--          using statements
--          cpp blocks
--          definitions

UnitFileGenerator = {

    new = function(args)
        if (args.library == nil) then
            error("No library argument given.");
        end
        if (args.unit == nil) then
            error("No library argument given.");
        end
        if (args.rootPath == nil) then
            error("No root path argument given.");
        end
        if (args.fileType == nil) then
            error("No file type argument given.");
        end
        args.libDecl = LibraryDecl(args.library)
        setmetatable(args, UnitFileGenerator);
        UnitFileGenerator.__index = function(t, k)
            local v = UnitFileGenerator[k];
            return v;
        end;
        return args;
    end,

    writeConfigFile = function(self)
        WriteLibraryConfigFileInclude(self.library, self.writer)
    end,

    writeClosing = function(self)
        WriteUnitFileClosing(self.unit, self.writer, self.fileType)
    end,

    writeOpening = function(self)
        WriteUnitFileOpening(self.unit, self.writer, self.fileType)
        -- TODO: Iterate all blocks, if they are "top" blocks write them here.
        if "Cpp" == self.fileType then
            self.writer:WriteLine(
                "// The following configures symbols for export if needed.")
            self.writer:WriteLine("#define " .. LibraryCreate(self.library) .. "\n");
            -- No need to #include the header for this unit- the various items
            -- will include their own definition.
        else
            -- Header file.
        end
        -- In order for the global functions to work properly, this will be
        -- necessary here. Otherwise every single stinkin' Element would need
        -- to do this, and since there is one per library that would be
        -- pretty darn crazy right?!!?!!
        self:writeConfigFile()

        -- TODO: Write all block import statements.

    end,


    WriteFile = function(self)
        local fileProp = self.fileType .. "File"  -- HFile or CppFile
        local setFunc = "Set" .. self.fileType .. "FileRootDirectory"
        local requiresFileProp = "Requires" .. self.fileType .. "File"

        -- The unit file will be some kind of relative path with an empty root
        -- path, such as "", "/Company/Namespace/blah.h".
        -- This code changes the root path part to the output path sent into
        -- this function (self.rootPath), which will be something like
        -- "C:\MyFiles\MyProject\target".
        if (self.unit[fileProp] == nil) then
            log:Error("Unit " .. tostring(self.unit) .. " has no " .. self.fileType
                      .. "File!")
        end
        self.unit[setFunc](self.unit, self.rootPath);

        log:Write("Creating " .. self.fileType .. " file at "
                  .. tostring(self.unit[fileProp]));
        self.writer = self.unit[fileProp]:CreateFile();

        self:writeOpening()

        local elements = CreateElementList(self.unit)

        for i = 1, #elements do
            log:Write(tostring(i) .. '=' .. tostring(elements[i]))
            local element = elements[i]
            if element[requiresFileProp] then
                local node = element.Node
                local typeName = node.TypeName

                local func = FileWriters[self.fileType][typeName]
                if func == nil then
                    error("No way to write the element " .. tostring(element)
                          .. ". fileType=" .. tostring(self.fileType)
                          .. ", typeName=" .. tostring(typeName))
                end
                func(self.library, node, self.writer, self.unit)
            else
                -- TODO: Find some way to disable the constant
                --       generation of the cpp file.
            end
        end

        self:writeClosing()
    end,

};


