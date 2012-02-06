-- Physical Organizer.  Creates unit targets for Elements.
require "Cpp/Common";

local UnitTarget = Macaroni.Model.Project.UnitTarget;
local TypeNames = Macaroni.Model.TypeNames;




Generator =
{
    findOrCreateTarget = function(self, element)
        local parentNode = element.Node.Node;
        if (parentNode ~= nil and parentNode.TypeName == TypeNames.Class) then
            -- If this is nested in a class, lets forget about it.
            return parentNode.Element.Owner
        end
        local t = self.defaultTarget:Unit(element.Node.FullName);
        print(t)
        return t;
    end,

    parseElement = function(self, element)
        if self:shouldHaveTarget(element) then
            local newTarget = self:findOrCreateTarget(element)
            element:SwitchOwner(newTarget);
        end
    end,

    shouldHaveTarget = function(self, element)
        if (element.Node.HFilePath ~= nil) then
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
        generator:parseElement(element)
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
