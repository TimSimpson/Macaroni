require "Cpp/Common";
require "Plugin";

local Access = Macaroni.Model.Cpp.Access;
local Context = Macaroni.Model.Context;
local Node = Macaroni.Model.Node;
local TypeNames = Macaroni.Model.TypeNames;

-- The older method of generating is to iterate by Node, and for each type
-- (class, function, etc) figure out whether or not to generate a file and
-- proceed from there.
-- That isn't quite flexible enough, so the new system is this. It iterates by
-- unit, building the header and cpp file for each one before proceeding.

UnitFileGenerator = {

    new = function(library, path)
        if (library == nil) then
            error("No library argument given.");
        end
        args = {}
        setmetatable(args, UnitFileGenerator);
        args.targetLibrary = library;
        args.rootPath = path;
        UnitFileGenerator.__index = function(t, k)
            local v = UnitFileGenerator[k];
            return v;
        end;
        return args;
    end,

    iterateUnits = function (self, library, path)
        for unit in Plugin.IterateChildDependencies(library) do
            self:writeUnitFiles(unit);
        end
    end,

    writeUnitFiles = function(self, unit)
        if not unit.Generated then
            log:Write("Skipping unit " .. tostring(unit) .. ".")
            return
        end
        log:Write("Writing unit files for " .. tostring(unit) .. ".")
        self:writeUnitHppFile(unit)
        self:writeUnitCppFile(unit)
    end,

    writeUnitCppFile = function(self, unit)
        log:Write("Creating cpp file at " .. tostring(unit.CppFile));
        if (unit.CppFile == nil) then
            log:Error("Unit " .. tostring(unit) .. " has no CppFile!")
        end

        -- TODO: Everything!
        --local writer = unit.HFile:CreateFile();

        -- self.rootPath:NewPath(unit.Name)
    end,

    writeUnitHppFile = function(self, unit)
        log:Write("Creating hpp file at " .. tostring(unit.HFile));
        if (unit.CppFile == nil) then
            log:Error("Unit " .. tostring(unit) .. " has no HFile!")
        end
        local writer = unit.HFile:CreateFile();

        local elements = unit:CreateElementList()
        for i = 1, #elements do
            print(tostring(i) .. '=' .. tostring(elements[i]))
        end
        -- self.rootPath:NewPath(unit.Name)
        -- Get all elements,
        -- for each generate by passing the writer to them.

    end,


};
