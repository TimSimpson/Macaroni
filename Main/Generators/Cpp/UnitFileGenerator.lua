require "Cpp/Common";
require "Plugin";
require "Cpp/ClassCppFileGenerator";
require "Cpp/ClassHFileGenerator";
local CppEnumFileGenerator = require "Cpp/EnumFileGenerator";
require "Cpp/TypedefFileGenerator";
require "Cpp/UnitBlockGenerator";

local Access = require "Macaroni.Model.Cpp.Access";
local Context = require "Macaroni.Model.Context";
local EnumFileGenerator = CppEnumFileGenerator.EnumFileGenerator
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
            gen = EnumFileGenerator.new{node=node, targetLibrary=library,
                                        writer=writer};
            gen:parse()
            writer:Write('\n');
        end,

        --TODO: Make functions owned by Targets and then this will work.
        -- Function = function(library, node, writer, unit)
        --     gen = UnitFunctionGenerator.Write{node=node, unit=unit,
        --         targetLibrary=library, writer=writer};

        -- end,

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

        local elements = unit:CreateElementList()
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
