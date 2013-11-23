require "Cpp/Common";
require "Cpp/CppFileGenerator";
require "Cpp/HFileGenerator";
require "Macaroni.Model.Library";
require "Macaroni/MhFileGenerator";
require "Log";
require "Plugin"
-- Generates the Macaroni header file.  This single files contains abbreviated
-- info about an entire library.

targetLibrary = nil;

function GetMethod(name)
    if name == "Generate" then
        return
        {
            Describe = function(args)
                validateArgs(args)
                args.output.WriteLine(
                    [[Creates an "Interface" file at the path ]]
                    .. args.interfacePath .. [[ to the current library ]]
                    .. args.library .. [[. ]]
                    .. [[ Interface files are Macaroni source code that only ]]
                    .. [[contain class names with ~hfile references. ]])
            end,
            Run = function(args)
                validateArgs(args)
                Generate(args)
            end
        }
    end
end

function validateArgs(self)
    Plugin.Check(self.library ~= nil, "Missing argument 'library'.")
    Plugin.Check(self.interfacePath ~= nil, "Missing argument 'interfacePath'.")
    Plugin.Check(self.output ~= nil, "Missing argument 'output'.")
end

function Generate(self)
	log.Init("InterfaceMh");
	log.Write = function(self2, msg)
			-- print("[InterfaceMh]:" .. msg);
		end;
    log:Write( "Generating Macaroni Header File\n");
    targetLibrary = self.library;
    local gen = MhFileGenerator.new(self.output, self.interfacePath,
                                    self.library);
    gen:iterate(self);
end

-- function Install(library, sourcePaths, outputPath, installPath, extraArgs)
-- 	local dstPath = installPath:NewPathForceSlash("Mh");
-- 	local interfacePath = outputPath:NewPathForceSlash("Interface.mh");
-- 	interfacePath:CopyToDifferentRootPath(dstPath);
-- end
