require "Cpp/Common";
require "Plugin";
require "Cpp/ClassCppFileGenerator";
require "Cpp/ClassHFileGenerator";
local CppEnumFileGenerator = require "Cpp/EnumFileGenerator";
require "Cpp/TypedefFileGenerator";
require "Cpp/UnitBlockGenerator";

local Access = require "Macaroni.Model.Cpp.Access";
local Context = require "Macaroni.Model.Context";
local EnumHFileGenerator = CppEnumFileGenerator.EnumHFileGenerator
local EnumCppFileGenerator = CppEnumFileGenerator.EnumCppFileGenerator
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
            gen = ClassHFileGenerator.new{node = node, targetLibrary=library,
                                          writer=writer};
            gen:parse()
            writer:Write('\n'); -- avoid bunching up typedefs
        end,

        Enum = function(library, node, writer)
            gen = EnumHFileGenerator.new{node=node, targetLibrary=library,
                                        writer=writer};
            gen:parse()
            writer:Write('\n');
        end,

        --TODO: Make functions owned by Targets and then this will work.
        FunctionOverload = function(library, node, writer, unit)
            -- TODO: Actually do this!
            --gen = UnitFunctionGenerator.Write{node=node,
            --    targetLibrary=library, writer=writer};
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

        FunctionOverload = function(library, node, writer, unit)
            -- TODO: Actually do this!
            --gen = UnitFunctionGenerator.Write{node=node,
            --    targetLibrary=library, writer=writer};
        end,
    },
};

UnitFileGenerator = {

    new = function(library)
        if (library == nil) then
            error("No library argument given.");
        end
        args = {}
        setmetatable(args, UnitFileGenerator);
        args.targetLibrary = library;
        -- if path == nil then
        --     error("Argument #2, 'path', cannot be nil.")
        -- end
        -- args.rootPath = path;
        UnitFileGenerator.__index = function(t, k)
            local v = UnitFileGenerator[k];
            return v;
        end;
        return args;
    end,

    createElementList = function(self, unit)
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
    end,

    iterateUnits = function (self, library, rootPath)
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
        self:writeUnitHppFile(unit, rootPath)
        self:writeUnitCppFile(unit, rootPath)
    end,

    writeUnitFile = function(self, unit, rootPath, fileType)
        local fileProp = fileType .. "File"  -- HFile or CppFile
        local setFunc = "Set" .. fileType .. "FileRootDirectory"
        local requiresFileProp = "Requires" .. fileType .. "File"

        -- The unit file will be some kind of relative path with an empty root
        -- path, such as "", "/Company/Namespace/blah.h".
        -- This code changes the root path part to the output path sent into
        -- this function (rootPath), which will be something like
        -- "C:\MyFiles\MyProject\target".
        if (unit[fileProp] == nil) then
            log:Error("Unit " .. tostring(unit) .. " has no " .. fileType
                      .. "File!")
        end
        unit[setFunc](unit, rootPath);

        log:Write("Creating " .. fileType .. " file at "
                  .. tostring(unit[fileProp]));
        local writer = unit[fileProp]:CreateFile();

        local elements = self:createElementList(unit)

        for i = 1, #elements do
            log:Write(tostring(i) .. '=' .. tostring(elements[i]))
            local element = elements[i]
            if element[requiresFileProp] then
                local node = element.Node
                local typeName = node.TypeName

                local func = FileWriters[fileType][typeName]
                if func == nil then
                    error("No way to write the element " .. tostring(element)
                          .. ". fileType=" .. tostring(fileType)
                          .. ", typeName=" .. tostring(typeName))
                end
                func(self.targetLibrary, node, writer, unit)
            else
                -- TODO: Find some way to disable the constant
                --       generation of the cpp file.
            end
        end
    end,

    writeUnitCppFile = function(self, unit, rootPath)
        self:writeUnitFile(unit, rootPath, "Cpp")
    end,

    writeUnitHppFile = function(self, unit, rootPath)
        self:writeUnitFile(unit, rootPath, "H")
    end,


};
