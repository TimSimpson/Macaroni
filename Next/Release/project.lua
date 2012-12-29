require "os"
require "Macaroni.IO.Path"


upper = getUpperProject();

--dependency {group=upper.Group, name="Macaroni.Tests.Bugs", version=upper.Version}
--dependency {group="Macaroni.Examples", name="Hello", version="1.0.0.0"}

sources = { "source/scripts", "source/www" }
outputPath = "target"

local version = upper.Version
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
    -- Copy all files from the src directory into the dst directory
    -- if they match regex.
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

function createDistributionDirectory(jamConfig, ext, dstDir, errorHint)
    local macaroniBinary = newPath(
        "../../Main/App/PureCpp/bin/" .. jamConfig
         .. "/link-static/threading-multi"):NewPath("/macaroni_p" .. ext)
    if not macaroniBinary.Exists then
        output:ErrorLine("Missing file " .. macaroniBinary.AbsolutePath .. "!")
        output:ErrorLine("Hint:" .. errorHint)
        error("Missing Macaroni binary.")
    end
    createDistributionDirectoryBase(dstDir)
    macaroniBinary:CopyTo(dstDir:NewPathForceSlash("macaroni" .. ext), true);
    newPath("../../Main/App/Source/main/resources"):NewPath("/Messages.txt")
        :CopyToDifferentRootPath(dstDir, true);
end

function createDistribution(jamConfig, ext, dstPath)
    local dstDir = newPath(dstPath);
    createDistributionDirectory(jamConfig, ext, dstDir)
    zipDirectory(dstDir)
end


function createDistributionDirectoryBase(dstDir)
    copyFiles(newPath("../../Main"):NewPath("/Generators"),
              dstDir, [[\.lua?$]]);
    copyFiles(newPath("../../Main"):NewPath("/Libraries"),
              dstDir, [[\.(jam|lua|mh)?$]]);
end


function createPureCpp()
    local dstDir = newPath("target/macaroni-" .. version .. "-pureCpp");
    copyFiles(newPath("../../Main/App/PureCpp"), dstDir,
            [[\.(jam|h|cpp|ico|txt|rc|rc2)?$]], "bin")
    zipDirectory(dstDir);
end

function generate()
end

built = false

function build()
    if built then return end
    built = true
    load("Macaroni.Examples", "Hello", "1.0.0.0")
    load("Macaroni.Examples", "Blocks", "1.0.0.0")
    load("Macaroni", "Macaroni.Tests.Features.AccessTypes.Lib", version)
    load("Macaroni", "Macaroni.Tests.Features.LuaGlue", version)

    createDistribution(
        "gcc-mingw-4.7.2/release", ".exe",
        "target/macaroni-" .. version .. "-windows",
        [[
        Enter the pure Cpp directory in Windows and execute:
        $ bjam -d+2 -j8 --toolset=gcc cxxflags=-std=gnu++11 link=static threading=multi release
        ]])
    createDistribution(
        "gcc-mingw-4.7.2/debug", ".exe",
        "target/macaroni-" .. version .. "-windows-debug",
        [[
        Enter the pure Cpp directory in Windows and execute:
        $ bjam -d+2 -j8 --toolset=gcc cxxflags=-std=gnu++11 link=static threading=multi
        ]])

    createDistribution(
        "gcc-4.7/release/address-model-32", "",
        "target/macaroni-" .. version .. "-ubuntu-32",
        [[
        In the Ubuntu 32 VM, execute
        /vagrant/mbuild build.sh release
        ]])
    createDistribution(
        "gcc-4.7/debug/address-model-32", "",
        "target/macaroni-" .. version .. "-ubuntu-32-debug",
        [[
        In the Ubuntu 32 VM, execute
        /vagrant/mbuild build.sh
        ]])

    createDistribution(
        "gcc-4.7/release/address-model-64", "",
        "target/macaroni-" .. version .. "-ubuntu-64",
        [[
        In the Ubuntu 64 VM, execute
        /vagrant/mbuild build.sh release
        ]])
    createDistribution(
        "gcc-4.7/debug/address-model-64", "",
        "target/macaroni-" .. version .. "-ubuntu-64-debug",
        [[
        In the Ubuntu 64 VM, execute
        /vagrant/mbuild build.sh
        ]])

    createPureCpp();
    local site = plugins:Get("Site")
    site:Run("build", {
        project=project,
        outputPath=newPath("target"),
        sources={newPath("source/www")},
        output=output,
        context=context
    });
    -- local versionDownloads = dir:NewPathForceSlash(
    --      "target/www/site/downloads/" ..version);
    -- versionDownloads:CreateDirectory();
    -- print("TODO: Zip up distirubiton directory and pure CPP directory and place in "
    --      .. tostring(versionDownloads) .. '.');
    -- print("TODO: Zip up the directory " .. tostring(versionDownloads)
    --      .. " - it is your release artifact.");
end

function install()
    build()
end
