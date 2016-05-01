-- Physical Organizer.  Creates unit targets for Elements.
require "Cpp/Common";

local UnitTarget = require "Macaroni.Model.Project.UnitTarget";
local TypeNames = Macaroni.Model.TypeNames;

-- Parses NodeSpace for a library.
-- Each element that does not have an For each element which deserves one
-- has a new Unit target created for it with the name of the element Node.
-- Later the units will be generated into C++ source with the Cpp plugin.


Generator =
{
    findOrCreateTarget = function(self, path, element)
        local t = self.defaultTarget:Unit(element.Node.FullName, true);
        local fileName = element.Node:GetPrettyFullName("/")
        t:SetHFileAsUnknownRelativePath(fileName .. ".h")
        t:SetCppFileAsUnknownRelativePath(fileName .. ".cpp")
        return t;
    end,

    parseElement = function(self, path, element)
        if self:shouldHaveTarget(element) then
            local newTarget = self:findOrCreateTarget(path, element)
        element:SwitchOwner(newTarget);
        end
    end,


    shouldHaveTarget = function(self, element)
        if element.TypeName ~= "Class" and element.TypeName ~= "Typedef"
            and element.TypeName ~= "Enum" then
            return false;
        end
        --if (element.Node.HFilePath ~= nil or (not element.RequiresCppFile)) then
        if (element.Node.HFilePath ~= nil) then
            return false;
        end
        -- If this element was adopted then it doesn't handle its own
        -- unit no matter what.
        if element.Node.AdoptedHome ~= nil then
            return false;
        end
        -- Inner classes or typedefs owned by classes should be defined in
        -- the class generators. Logically they are distinct, but physically
        -- speaking its all one giant unit.
        local parentNode = element.Node.Node;
        if (parentNode ~= nil and parentNode.TypeName == TypeNames.Class) then
            -- If this is nested in a class, lets forget about it.
            return false;
        end
        return true;
    end,

};

function Generate(libTarget, path)

    local generator = Generator;
    generator.defaultTarget = libTarget;

    local elements = libTarget:CreateElementList();
    for i=1, #elements do
        local element = elements[i]
        if (element:OwnedBy(libTarget)) then
            generator:parseElement(path, element)
        end
    end
end

function GetMethod(name)
    if name == "Generate" then
        return
        {
            Describe = function(args)
                args.output.WriteLine("Generate Targets for Nodes in "
                                      .. tostring(args.target) .. ".");
            end,
            Install = function(args)
                -- By default, Macaroni itself will copy all artifacts
                -- and source to the destination directory "Source".
            end,
            Run = function(args)
                Generate(args.target, args.path)
            end
        }
    end
    return nil;
end
