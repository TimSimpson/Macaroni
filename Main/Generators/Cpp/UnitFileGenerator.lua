require "Cpp/Common";
require "Plugin";
require "Cpp/ClassCppFileGenerator";
require "Cpp/ClassHFileGenerator";
local EnumFileGenerator = require "Cpp/EnumFileGenerator";
local FunctionGenerator = require "Cpp/FunctionFileGenerator";
require "Cpp/TypedefFileGenerator";
require "Cpp/UnitBlockGenerator";

local Access = require "Macaroni.Model.Cpp.Access";
local CodeGraph = require "Macaroni.Generators.Cpp.CodeGraph"
local Context = require "Macaroni.Model.Context";
local EnumFileGenerator = EnumFileGenerator.EnumFileGenerator
local EnumCppFileGenerator = EnumFileGenerator.EnumCppFileGenerator
local FunctionFileGenerator = FunctionGenerator.FunctionFileGenerator
local LibraryTarget = require "Macaroni.Model.Project.LibraryTarget"
local Node = require "Macaroni.Model.Node";
local UnitFileGenerator = require "Macaroni.Generators.Cpp.Unit.UnitFileGenerator"
local TypeNames = Macaroni.Model.TypeNames;

-- The older method of generating is to iterate by Node, and for each type
-- (class, function, etc) figure out whether or not to generate a file and
-- proceed from there.
-- That isn't quite flexible enough, so the new system is this. It iterates by
-- unit, building the header and cpp file for each one before proceeding.

FileWriters = {
    Block = function(library, node, writer, type)
        return UnitBlockGenerator.new{
            writer=writer, node=node, type=type};
    end,

    Class = function(library, node, writer, type)
        if type == "H" then
             return ClassHFileGenerator.new{
                node = node, targetLibrary=library, writer=writer};
        else
            return ClassCppFileGenerator.new{
                node = node, targetLibrary=library, writer=writer};
        end
    end,

    Enum = function(library, node, writer, type)
        return EnumFileGenerator.new{
            node=node, targetLibrary=library, writer=writer,
        };
    end,

    FunctionOverload = function(library, node, writer, type)
        return FunctionFileGenerator.new{
                node = node, targetLibrary=library, writer=writer,
                fileType = type,
                insertIntoNamespaces=true,
                ownedByClass=false,
                classPrefix=nil,
        };
    end,

    Typedef = function(library, node, writer, type)
        return TypedefFileGenerator.new{node=node, targetLibrary=library,
                                        writer=writer};
    end,
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
    if fileType == "Cpp" then
        local hGuardName = UnitGuardName(unit, "H")
        writer:WriteLine("// Force the use of the Internal Header:")
        writer:WriteLine("#define " .. hGuardName)
    end
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
        self = {}
        setmetatable(self, LibraryTargetGenerator);
        self.library = library;
        -- if path == nil then
        --     error("Argument #2, 'path', cannot be nil.")
        -- end
        -- self.rootPath = path;
        self.graph = CodeGraph.Create();
        self.libDecl = LibraryDecl(self.library)
        LibraryTargetGenerator.__index = function(t, k)
            local v = LibraryTargetGenerator[k];
            return v;
        end;
        return self;
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

        -- NEW!
        local ug = UnitFileGenerator.CreateDebug(self.library, unit);
        -- local ug = UnitFileGenerator.Create(self.library, unit);
        ug:Generate(self.graph, rootPath);
        -- EXCITING!

        -- local h   = UnitFileGeneratorOldLua.new(args);
        -- h:WriteFile();
        -- args.fileType = "Cpp"
        -- local cpp = UnitFileGeneratorOldLua.new(args);
        -- cpp:WriteFile();
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

UnitFileGeneratorOldLua = {

    new = function(self)
        if (self.library == nil) then
            error("No library argument given.");
        end
        if (self.unit == nil) then
            error("No library argument given.");
        end
        if (self.rootPath == nil) then
            error("No root path argument given.");
        end
        if (self.fileType == nil) then
            error("No file type argument given.");
        end
        self.libDecl = LibraryDecl(self.library)
        setmetatable(self, UnitFileGeneratorOldLua);
        UnitFileGeneratorOldLua.__index = function(t, k)
            local v = UnitFileGeneratorOldLua[k];
            return v;
        end;
        self.elements = CreateElementList(self.unit)
        self.requiresFileProp = "Requires" .. self.fileType .. "File"
        return self;
    end,

    foreachWriter = function(self, func)
        for i, w in ipairs(self.elementWriters) do
            if w[func] ~= nil then
                w[func](w)
            end
        end
    end,

    requiresFile = function(self, element)
        if MACARONI_VERSION ~= "0.2.3" then
            -- TODO: Fix the bug in Block which makes it not understand if it
            --       does or does not require a file property.
            if element.TypeName == "Block" then
                return true
            else
                return element[self.requiresFileProp]
            end
        else
            return element[self.requiresFileProp]
        end
    end,

    writeConfigFile = function(self)
        WriteLibraryConfigFileInclude(self.library, self.writer,
                                      self.fileType~="Cpp")
    end,

    writeClosing = function(self)
        self:foreachWriter('WriteBottomBlocks');
        WriteUnitFileClosing(self.unit, self.writer, self.fileType)
    end,

    writeForwardDeclarations = function(self)
        self.writer:WriteLine("// Forward declaration necessary for anything which also depends on this.\n");

        -- for i = 1, #elements do
        --     log:Write(tostring(i) .. '=' .. tostring(elements[i]))
        --     local element = elements[i]
        --     if element[requiresFileProp] then
        --         local node = element.Node
        --         local typeName = node.TypeName

        --         local func = FileWriters[self.fileType][typeName]
        --         if func == nil then
        --             error("No way to write the element " .. tostring(element)
        --                   .. ". fileType=" .. tostring(self.fileType)
        --                   .. ", typeName=" .. tostring(typeName))
        --         end
        --         func(self.library, node, self.writer, self.unit)
        --     else
        --         -- TODO: Find some way to disable the constant
        --         --       generation of the cpp file.
        --     end
        -- end
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

        self:foreachWriter('WriteTopBlocks')

        --TODO: Write "top" blocks
        --self:writeConfigFile()
    end,

    writeHeader = function(self)
        self:writeConfigFile()
        self:foreachWriter('WriteForwardDeclarations');
        self:foreachWriter('WriteIncludeStatements')
        self:foreachWriter('WriteForwardDeclarationsOfDependencies');
        self:foreachWriter('WritePreDefinitionBlocks');
        self:foreachWriter('WriteHeaderDefinitions');
        self:foreachWriter('WritePostDefinitionBlocks');
        -- for i, w in ipairs(self.elementWriters) do
        --     self.elementWriters[i].WriteHeader()
        -- end
    end,

    writeImplementation = function(self)
        self.writer:WriteLine("/*--------------------------------------------------------------------------*");
        self.writer:WriteLine(" * Internal Header                                                         *");
        self.writer:WriteLine(" *--------------------------------------------------------------------------*/");
        self:writeHeader();
        self.writer:WriteLine("\n");
        self.writer:WriteLine("/*--------------------------------------------------------------------------*");
        self.writer:WriteLine(" * Implementation                                                        *");
        self.writer:WriteLine(" *--------------------------------------------------------------------------*/");
        self:foreachWriter('WriteImplementationIncludeStatements')
        self:foreachWriter('WriteUsingStatements')
        self:foreachWriter('WriteImplementation')
    end,


    WriteFile = function(self)
        local fileProp = self.fileType .. "File"  -- HFile or CppFile
        local setFunc = "Set" .. self.fileType .. "FileRootDirectory"

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

        -- Initialize all of the Element writers

        self.elementWriters = {}
        for i = 1, #self.elements do
            local element = self.elements[i]
            if self:requiresFile(element) then
                local node = element.Node
                local typeName = node.TypeName
                local func = FileWriters[typeName]
                if func == nil then
                    error("No way to write the element " .. tostring(element)
                          .. ". fileType=" .. tostring(self.fileType)
                          .. ", typeName=" .. tostring(typeName))
                end
                local writer = func(self.library, node, self.writer, self.fileType)
                self.elementWriters[#self.elementWriters + 1] = writer
            end
        end

        self:writeOpening()

        if self.fileType == "H" then
            self:writeHeader()
        else
            self:writeImplementation()
        end

        self:writeClosing()
    end,

};


