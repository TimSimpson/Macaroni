require "os"
local Path = require "Macaroni.IO.Path";



--dependency {group=upper.Group, name="Macaroni.Tests.Bugs", version=upper.Version}
--dependency {group="Macaroni.Examples", name="Hello", version="1.0.0.0"}

sources = { "source/scripts", "source/www" }
outputPath = "target"

local version = "0.4.4"
project = context:Group("Macaroni")
                 :Project("Macaroni.Release")
                 :Version(version)

local dir = Path.New(getWorkingDirectory())

function newPath(suffix)
    return dir:NewPathForceSlash(suffix):CreateWithCurrentAsRoot();
end

function copyRstFiles(src, dst)
    copyFiles(src, dst, [[\.(rst)?$]]);
end

function copyCppFiles(src, dst)
    copyFiles(src, dst, [[\.(c|cpp|h|hpp)?$]]);
end

function copyFiles(src, dst, regex)
    -- Copy all files from the src directory into the dst directory
    -- if they match regex.
    if not dst then error("dst arg was nil", 2) end
    local filePaths = src:GetPaths(regex)
    for i = 1, #filePaths do
        local fp = filePaths[i];
        local copy = true;
        if copy then
            fp:CopyToDifferentRootPath(dst, true);
        end
        -- print(tostring(fp));
    end
end

function copyJamFiles(src, dst)
    copyFiles(src, dst, [[\.jam?$]]);
end

function copyResourceFiles(src, dst)
    copyFiles(src, dst, [[\.(ico|rc|rc2|h)?$]]);
end

function zipDirectory(dstPath)
    local scriptPath = newPath("source/scripts/zip_it.py")
    cmd = "python "
        .. " " .. scriptPath.AbsolutePath
        .. " " .. dstPath.AbsolutePath
        .. " " .. dstPath.AbsolutePath;
    os.execute(cmd)
end

function createDistributionDirectory(args)
    local macaroniBinary = args.macaroniBinary
    if not macaroniBinary then
        macaroniBinary = newPath(
        "../../Main/App/PureCpp/bin/" .. args.binPath
         .. "/link-static/threading-multi"):NewPath("/macaroni"
        .. args.weirdTag .. args.ext)
    end
    if not macaroniBinary.Exists then
        output:ErrorLine("Missing file " .. macaroniBinary.AbsolutePath .. "!")
        output:ErrorLine("Hint:" .. args.errorHint)
        error("Missing Macaroni binary.")
    end
    createDistributionDirectoryBase(args.dstDir)
    macaroniBinary:CopyTo(args.dstDir:NewPathForceSlash("macaroni" .. args.ext), true);
    newPath("../../Main/App/Source/main/resources"):NewPath("/Messages.txt")
        :CopyToDifferentRootPath(args.dstDir, true);
end


function createDistributionDirectoryAndZip(args)
    local dstPath = "target/macaroni-" .. version .. args.tag
    args.dstDir = newPath(dstPath);
    createDistributionDirectory(args);
    zipDirectory(args.dstDir)
end


function createDistributionArgs(args)
    local distArgs = nil
    if args.windows then
        distArgs = {
                binPath = "msvc-12.0", ext = ".exe", tag = "-windows",
                weirdTag = "_p",
                errorHint = [[
    Enter the pure Cpp directory in Windows and execute:
    bjam -d+2 -j8 --toolset=msvc-12.0 link=static threading=multi address-model=32 release]]
               }
    else
        distArgs = {
                 binPath = "gcc-linux", ext = "", tag="-ubuntu", weirdTag = "_p",
                 errorHint = [[
    In the Ubuntu ]] .. tostring(args.bits) .. [[ VM, execute
    /vagrant/mbuild build.sh]]
              }
    end
    if args.release then
        distArgs.binPath = distArgs.binPath .."/release"
        distArgs.errorHint = distArgs.errorHint .. " release"
    else
        distArgs.binPath = distArgs.binPath .. "/debug"
        distArgs.tag = distArgs.tag .. "-debug"
    end
    if args.bits then
        distArgs.binPath = distArgs.binPath .. "/address-model-"
                           .. tostring(args.bits)
        distArgs.tag = distArgs.tag .. "-" .. tostring(args.bits)
    end
    return distArgs;
end

function createUniqueDistribution(args)
    -- Use BJAM to make a special distribution for a specific platform.
    local distArgs = createDistributionArgs(args)
    createDistributionDirectoryAndZip(distArgs);
end

function createDistributionDirectoryBase(dstDir)
    if not dstDir then error("dstDir was nil", 2) end
    copyFiles(newPath("../../Main"):NewPath("/Generators"),
              dstDir, [[\.lua?$]]);
    copyFiles(newPath("../../Main"):NewPath("/Libraries"),
              dstDir, [[\.(jam|lua|mh)?$]]);
end


function createPureCpp()
    local dstDir = newPath("target/macaroni-" .. version .. "-pureCpp");
    copyFiles(newPath("../../Main/App/PureCpp"), dstDir,
            [[\.(jam|h|hpp|cpp|ico|txt|rc|rc2)?$]], "bin")
    zipDirectory(dstDir);
end

function loadWebSiteExamples()
    -- Loads projects which are referenced by the web site.
    load("Macaroni.Examples", "Hello", "1.0.0.0")
    load("Macaroni.Examples", "Blocks", "1.0.0.0")
    load("Macaroni", "Macaroni.Tests.Features.AutoDeclAndTrt", version)
    load("Macaroni", "Macaroni.Tests.Features.AccessTypes.Lib", version)
    load("Macaroni", "Macaroni.Tests.Features.LuaGlue", version)
end

function buildWebSite()
    loadWebSiteExamples();

    local target=newPath("target")
    local site = plugins:Get("Site")
    site:Run("build", {
        project=project,
        outputPath=target,
        sources={newPath("source/docs")},
        output=output,
        context=context,
        extension="rst",
        outputSubPath="/docs",
    });


    local dstRoot = target:NewPathForceSlash("docs")
    local function addDocs(group, name, version, dstName)
        load(group, name, version)
        local otherProject = context:FindProjectVersion(
            group, name, version);
        if otherProject == nil then error("Can't find project "
            .. group .. "/" .. name .. "/" .. version .."!") end
        local installPath = findInstallPath(otherProject);
        if (installPath == nil) then
            error("Could not find install path for library "
                  .. pvIdToString(projectVersionId));
        end
        local srcPath = installPath:NewPathForceSlash("target/docs");
        if (not srcPath.Exists) then
            error("No target/docs path found for project "
                  .. tostring(otherProject) .. ".");
        end
        local dstPath = dstRoot:NewPathForceSlash(dstName)
        print(srcPath)
        print(dstPath)
        dstPath:CreateDirectory();
        copyRstFiles(srcPath:CreateWithCurrentAsRoot(), dstPath);
    end

    addDocs("Macaroni.Examples", "Blocks", "1.0.0.0",
            "reference/code/blocks")
    addDocs("Macaroni", "Macaroni.Tests.Features.AutoDeclAndTrt", version,
            "reference/code/autoDeclAndTrt")
    addDocs("Macaroni", "Macaroni.Tests.Features.Dependencies", "DEV",
            "reference/code/dependencies")
    addDocs("Macaroni", "Macaroni.Tests.Features.Enum", "DEV",
             "reference/code/enum")
    addDocs("Macaroni", "Macaroni.Tests.Features.LuaGlue", version,
            "reference/plugins/luaglue")
    os.execute("sphinx-build  -b html "
        .. target:NewPathForceSlash("docs").AbsolutePath .. " "
        .. target:NewPathForceSlash("site").AbsolutePath
    )

end

function zipWebSite()
    local dstDir = newPath("target/site");
    zipDirectory(dstDir);
end


function clean()
    newPath("target"):ClearDirectoryContents();
end

function generate()
end

built = false

function build()
    if built then return end
    built = true

    createPureCpp();

    buildWebSite();

    zipWebSite();

    local binDir = newPath("../../Main/App/GeneratedSource/exe");
    local linuxBin = binDir:NewPathForceSlash("macaroni");
    local winBin = binDir:NewPathForceSlash("macaroni.exe");


    local bin = nil
    local tag = nil
    local ext = nil
    if winBin.Exists then
        bin = winBin
        tag = "-windows"
        ext = ".exe"
    else
        bin = linuxBin
        tag = "-linux"
        ext = ""
    end

    createDistributionDirectoryAndZip{
        macaroniBinary=bin,
        tag=tag,
        bits=32,
        ext=ext,
        errorHint="Shrug Emoji",
    };


    -- createUniqueDistribution{windows=true,  release=false, bits=32 };
    -- createUniqueDistribution{windows=true,  release=true, bits=32  };
    --createUniqueDistribution{windows=false, release=false, bits=32 };
    -- createUniqueDistribution{windows=false, release=true,  bits=32 };
    -- createUniqueDistribution{windows=false, release=false, bits=64 };
    -- createUniqueDistribution{windows=false, release=true,  bits=64 };
end

function document()
    -- Ignore
end

function install()
    build()
    sinstall(project, filePath("./"));
    print("Installed!")
end
