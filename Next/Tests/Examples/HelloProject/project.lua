-- A test project for the new Project system.
FileSet = Macaroni.IO.FileSet
Path = Macaroni.IO.Path
Files = Macaroni.Model.Project.Files

project = context:Group("Macaroni.Examples")
                       :Project("Hello-Project")
                       :Version("1.0.0.0");
assert(project.Version, "1.0.0.0")
assert(project.Project.Name, "Hello-Project")
assert(project.Project.Group.Name, "Macaroni.Examples")


-- Define our dependencies.
-- Normally we'd pull this from an external file.
-- load("Macaroni", "Boost-headers", "1.46.1")

-- Added to init.lua:
boost_version = properties.boost.version

boost_headers = context:
    Group("Macaroni"):Project("Boost-headers"):Version(boost_version)
assert(boost_headers:GetTarget("lib") == nil)
-- boost_headers_lib = boost_headers:HeaderOnlyLibrary(
--     { name = "lib",
--       dependencies = {},
--       includePath = { Path.New(properties.boost["1.46.1"].include ) },
--       macaroniSource = { FileSet.Create("src/deps/boost_1.46.1", ".mcpp$") }
--       });

boost_props = properties.boost["1.55.1"]
boost_headers_lib = boost_headers:Library{
  name = "lib",
  headers = pathList{boost_props.include},
  sources = pathList{},
}
print(boost_headers:GetTarget("lib"))
print(boost_headers_lib)
assert(boost_headers:GetTarget("lib"):GetCId() == boost_headers_lib:GetCId())
--assert(tostring(boost_headers_libpro.IncludePath[1])
--        == properties.boost["1.46.1"].include)
--assert(tostring(boost_headers_lib.MacaroniSource[1].Path)
--        == "src/deps/boost_1.46.1")
-- assert(boost_headers_lib.MacaroniSource[1].Regex == ".mcpp$")
-- assert(#boost_headers_lib.CppSource == 0)


-- assert(tostring(boost_headers_lib.Sources.Macaroni[1].Path)
--        == "src/deps/boost_1.46.1")
-- assert(#boost_headers_lib.Sources.Cpp == 0)

-- boost_regex = context:
--     Group("Macaroni"):Project("Boost-regex"):Version("1.46.1")
-- boost_regex_lib = boost_regex:BinaryLibrary(
--     { -- We can omit the name, as the default will be "lib".
--       dependencies = {},
--       includePath = { Path.New(properties.boost["1.46.1"].include ) },
--       macaroniSource = {
--         FileSet.Create("src/deps/boost_1.46.1-regex", ".mcpp$")
--       }
--     })
-- boost_regex_lib:AddBinary(
--     { config=Config.New{variant=Config.DEBUG},
--       directoryPath = { properties.boost["1.46.1"].stage }
--     });


-- Now create the library we're actually building.
lib = project:Library(
    {
        name = "lib",
        -- Important note: When the library sees a ProjectVersion as a
        -- dependency, it automatically uses a target named "lib".
        -- dependencies = {boost_regex_lib, boost_headers},
        headers = pathList{"src/main/mcpp"},
        sources = pathList{"src/main/mcpp", ".mcpp$"}

    }
)
assert(lib.ProjectVersion, project)
assert(lib.Name == "lib")
assert(lib.Sources.Macaroni ~= nil)
assert(#lib.Sources.Macaroni == 1)
assert(lib.Sources.Macaroni[1].Path, "src/main/mcpp")
assert(lib.Sources.Macaroni[1].Regex, ".mcpp$")

bjam = plugins:Get("bjam")
cpp = plugins:Get("Cpp")

local output = Path.New("target")

cpp:Run("Generate", { target=lib, path=output })



assert(context.Root:Find("Cat") ~= nil);
--project:ParseFiles(name = )

-- cpp:Run("Generate", { blah={a=3}, number=5,
--                       target = lib })



--task = cpp:Task("Generate", lib, {blah={a=3}})
--task.Run();

-- bjam:Run("Generate", lib, buildContext, {

-- })

-- local output = "target"

-- lib:Generate("Cpp", {output=output})

-- -- plugins as PluginFactory
-- --

-- local builder = BuildContext(lib, output)

-- cpp:Run("Generate", lib, { output=output} )

-- -- I like this style the best:
-- lib:Generate("Cpp", {output=output})

-- -- But this style seems more flexible

-- -- Properties is automatically added.
-- --



-- -- plugins stores AppPaths

-- cpp.Run("Generate", lib, {output=output})

-- print("Library:")
-- print(lib)


-- function install()

-- end

-- In the future try it this way:
-- lib = project:Library
-- {
-- 	name="lib",
--     sources= {FileSet.Create("src/main/mcpp", ".mcpp$")},
--     dependencies = {}
-- }
