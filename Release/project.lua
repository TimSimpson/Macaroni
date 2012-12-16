require "os"
require "Macaroni.IO.Path"

--upper = getUpperProject();

load("Macaroni.Examples", "Hello", "1.0.0.0")
load("Macaroni.Examples", "Blocks", "1.0.0.0")
load("Macaroni", "Macaroni.Tests.Features.AccessTypes.Lib", "0.1.0.25")
load("Macaroni", "Macaroni.Tests.Features.LuaGlue", "0.1.0.25")

--dependency {group=upper.Group, name="Macaroni.Tests.Bugs", version=upper.Version}
--dependency {group="Macaroni.Examples", name="Hello", version="1.0.0.0"}

sources = { "source/scripts", "source/www" }
outputPath = "target"

local version = "0.1.0.25"
project = context:Group("Macaroni")
                 :Project("Macaroni.Release")
                 :Version(version)

local dir = Macaroni.IO.Path.New(getWorkingDirectory())

function newPath(suffix)
    return dir:NewPathForceSlash(suffix):CreateWithCurrentAsRoot();
end

function copyCppFiles(src, dst)
    copyFiles(src, dst, [[\.(c|cpp|h|hpp)?$]]);
end

function copyFiles(src, dst, regex)
    local filePaths = src:GetPaths(regex)
    for i = 1, #filePaths do
        local fp = filePaths[i];
        fp:CopyToDifferentRootPath(dst, true);
        -- print(tostring(fp));
    end
end

function copyJamFiles(src, dst)
    copyFiles(src, dst, [[\.jam?$]]);
end

function copyResourceFiles(src, dst)
    copyFiles(src, dst, [[\.(ico|rc|rc2|h)?$]]);
end

function createDistributionDirectory(jamConfig, ext, dstPath)
    local dstDir = newPath(dstPath);
    createDistributionDirectoryBase(dstDir)
    newPath("../Main/App/bin/" .. jamConfig
             .. "/link-static/threading-multi"):NewPath("/macaroni" .. ext)
        :CopyToDifferentRootPath(dstDir, true);
    newPath("../Main/App/Source/main/resources"):NewPath("/Messages.txt")
        :CopyToDifferentRootPath(dstDir, true);
end

-- function createReleaseDistributionDirectory()
--     createDistributionDirectory("release", "target/macaroni-" .. version)
-- end
-- function createDebugDistributionDirectory()
--     createDistributionDirectory("debug",
--                                 "target/macaroni-" .. version .. "-debug")
--     -- local dstDir = newPath("target/macaroni-" .. version .. "-debug");
--     -- createDistributionDirectoryBase(dstDir)
--     -- newPath("../Main/App/GeneratedSource/release/debug"):NewPath("/macaroni.exe")
--     --     :CopyToDifferentRootPath(dstDir, true);
--     -- newPath("../Main/App/GeneratedSource/release/debug"):NewPath("/messages.txt")
--     --     :CopyToDifferentRootPath(dstDir, true);
-- end

function createDistributionDirectoryBase(dstDir)
    copyFiles(newPath("../Main"):NewPath("/Generators"), dstDir, [[\.lua?$]]);
    copyFiles(newPath("../Main"):NewPath("/Libraries"), dstDir, [[\.(jam|lua|mh)?$]]);
end


function createPureCpp()
    local dstDir = newPath("target/macaroni-" .. version .. "-pureCpp");
    copyCppFiles(newPath("../Main/Dependencies/Lua/target"), dstDir);
    copyCppFiles(newPath("../Main/App/Source/main/mcpp"), dstDir);
    copyResourceFiles(newPath("../Main/App/Source/main/resources"), dstDir);
    copyCppFiles(newPath("../Main/App/GeneratedSource"), dstDir);
    copyFiles(newPath("../Main/App/Source/main/pureCppExtraFiles"), dstDir, [[\.(jam|txt)?$]]);
end

function build()
    createDistributionDirectory(
        "gcc-mingw-4.7.2/release", ".exe",
        "target/macaroni-" .. version .. "-windows")
    createDistributionDirectory(
        "gcc-mingw-4.7.2/debug", ".exe",
        "target/macaroni-" .. version .. "-windows-debug")
    createDistributionDirectory(
        "gcc-4.7/release", "",
        "target/macaroni-" .. version .. "-ubuntu-64")
    createDistributionDirectory(
        "gcc-4.7/debug", "",
        "target/macaroni-" .. version .. "-ubuntu-64-debug")
    createDistributionDirectory(
        "gcc-4.7/debug/address-model-32/architecture-x86/instruction-set-i686", "",
        "target/macaroni-" .. version .. "-ubuntu-32-debug")
    createDistributionDirectory(
        "gcc-4.7/release/address-model-32/architecture-x86/instruction-set-i686", "",
        "target/macaroni-" .. version .. "-ubuntu-32")

    createPureCpp();
    local site = plugins:Get("Site")
    site:Run("build", {
        project=project,
        outputPath=newPath("target"),
        sources={newPath("source/www")},
        output=output,
        context=context
    });
    local versionDownloads = dir:NewPathForceSlash(
         "target/www/site/downloads/" ..version);
    versionDownloads:CreateDirectory();
    print("TODO: Zip up distirubiton directory and pure CPP directory and place in "
         .. tostring(versionDownloads) .. '.');
    print("TODO: Zip up the directory " .. tostring(versionDownloads)
         .. " - it is your release artifact.");
end
