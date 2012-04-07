-- Physical Organizer.  Creates unit targets for Elements.
require "Cpp/Common";

local UnitTarget = Macaroni.Model.Project.UnitTarget;
local TypeNames = Macaroni.Model.TypeNames;

-- Parses NodeSpace for a library.
-- Each element that does not have an For each element which deserves one
-- has a new Unit target created for it with the name of the element Node.
-- Later the units will be generated.


Generator =
{
    findOrCreateTarget = function(self, path, element)
        local parentNode = element.Node.Node;
        if (parentNode ~= nil and parentNode.TypeName == TypeNames.Class) then
            -- If this is nested in a class, lets forget about it.
            return parentNode.Element.Owner
        end
        local t = self.defaultTarget:Unit(element.Node.FullName, true);
        local fileName = element.Node:GetPrettyFullName("/")
        t:SetHFileAsUnknownRelativePath(fileName .. ".hpp")
        t:SetCppFileAsUnknownRelativePath(fileName .. ".cpp")
        print(t)
        return t;
    end,

    parseElement = function(self, path, element)
        if self:shouldHaveTarget(element) then
            local newTarget = self:findOrCreateTarget(path, element)
            element:SwitchOwner(newTarget);
        end
    end,


    shouldHaveTarget = function(self, element)
        if (element.Node.HFilePath ~= nil or (not element.RequiresCppFile)) then
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
        generator:parseElement(path, element)
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
                -- print("")
                -- print("TARGET=" .. tostring(args.target));
                -- print("PATH=" .. tostring(args.path));
                -- print("PATH.NewPath=" .. tostring(args.path.NewPath));

                Generate(args.target, args.path)
            end
        }
    end
    return nil;
end
