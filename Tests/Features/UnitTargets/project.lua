description = [[
The goal of this test is to establish granularity within a library by compiling
a simple C++ app to the same degree of specificity as could be achieved by
Boost-Build.

The "manifest" build system was very broad; libraries were defines as blobs of
C++ files which all got combined. The problem was Macaroni never established
how to do this, so it was impossible to write generators to say something such
as "A.cpp is used by B.cpp, and has a single unit test called A-Test.cpp". The
goal with granular UnitTargets it to be able to generate build files and other
goodies that can figure this out in a compatable fashion.

]]

import("Macaroni", "ProjectTemplates", "1")
require "SimpleProject"

SimpleProject{
  group="Macaroni.Tests",
  project="Features-UnitTargets",
  version="1.0.0.0",
  src="src",
  target="target",
  dependencies = {
    load("Macaroni", "Boost-headers", "1.52"):Target("lib"),
    load("Macaroni", "CppStd", "2003"):Target("lib")
  },
};


-- project = context:Group("Macaroni.Tests")
--                        :Project("Features-UnitTargets")
--                        :Version("1.0.0.0");

-- boost  = load("Macaroni", "Boost-headers", "1.46.1"):Target("lib")
-- cppstd = load("Macaroni", "CppStd", "2003"):Target("lib")

-- lib = project:Library{
--     name = "lib",
--     -- Important note: When the library sees a ProjectVersion as a
--     -- dependency, it automatically uses a target named "lib".
--     -- dependencies = {boost_regex_lib, boost_headers},
--     headers = pathList{"src", "target"},
--     sources = pathList{"src", ".mcpp$"},

--     dependencies = {
--         cppstd,
--         boost
--     }
-- }

-- -- This would work, except that PlainCTest.cpp is already added above.
-- -- local src = Macaroni.IO.Path.New("src")
-- -- lib:AddTest("PlainCTest", src:NewPathForceSlash("PlainCTest.cpp"))

-- porg = plugins:Get("Porg")
-- cpp = plugins:Get("Cpp")
-- html = plugins:Get("HtmlView")
-- bjam = plugins:Get("BoostBuild2")


-- targetDir = Macaroni.IO.Path.New("target")

-- -- This *needs* to run every single time, no matter what.
-- porg:Run("Generate", {target=lib})

-- function clean()
--     targetDir:ClearDirectoryContents();
-- end

-- generated = false
-- built = false
-- installed = false

-- function generate()
--   if generated then return end
--   local outputPath = filePath("target") --ath.New("target")
--   cpp:Run("Generate", { projectVersion=project, path=outputPath })
--   html:Run("Generate", { target=lib, path=outputPath})
--   bjam:Run("Generate", { jamroot=outputPath:NewPath("/jamroot.jam"),
--                        projectVersion=project,
--                        output=output
--   })
--   generated = true
-- end

-- function build()
--   if built then return end
--   generate()
--   os.execute("bjam " .. properties.bjam_options .. " target")
--   built = true
-- end

-- function install()
--   if installed then return end
--   sinstall(project, filePath("./"))
--   installed = true


--   -- So, here's a laugh: what you see below are half-completed attempts at
--   -- feeling out what the proper way to do this would look like.
--   -- Before the sloppy and easy way was found.
--   --[[


--   generate()
--   --cpp:Run("Install", { projectVersion=project, paths=pathList{"src", "target"}})
--   -- Somehow, go through library, copy all unit files correctly, and create
--   -- new jamroot.jam file with library.
--   ----bjam:Run("Install", { projectVersion=project } );--{ libraries={lib,} })
--   installer = install(project, {lib});
--   installer:install({lib});

--   -- From Lua, an installer is its own object which contains the following:
--   --   FinalProject - a ProjectVersion which is independent of what is being
--   --                  installed, i.e. it is *NOT* the "project" varaible above.
--   --   OutputPath   - The install directory.

--   bjam:Run("Install", {installer = installer})
--   -- This will take the installer, from which it will extract the
--   -- projectVersion from the "project" variable, then it will use the
--   -- "OutputPath" variable and make a new directory called bjam and put
--   -- everything there.


--   -- Creates the final install path
--   -- Goes through all targets and saves unit targets with cpp and h files
--   -- to "Source" directory.
--   -- Writes installer file.
--   -- project:Install{{targets={lib}, plugins={bjam}};
--   --   Targets may have a special way they get installed.
--   --   Plugins need to save whatever they generate, so for example bjam needs
--   --   to save its own stuff.
--   ]]--
-- end
