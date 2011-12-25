-- A test project for the new Project system.
FileSet = Macaroni.IO.FileSet;

project = context:Group("Macaroni.Examples")
                       :Project("Hello-Project")
                       :Version("1.0.0.0");
print("Created project:")
print(project)


lib = project:Library(
    "lib",
    {},
    { FileSet.Create("src/main/mcpp", ".mcpp$") }
)

local bjam = plugins:Get("bjam")
local cpp = plugins:Get("Cpp")

local buildContext = BuildContext("target")
cpp:Run("Generate", lib, buildContext, {})


bjam:Run("Generate", lib, buildContext, {

})

local output = "target"

lib:Generate("Cpp", {output=output})

-- plugins as PluginFactory
--

local builder = BuildContext(lib, output)

cpp:Run("Generate", lib, { output=output} )

-- I like this style the best:
lib:Generate("Cpp", {output=output})

-- But this style seems more flexible

-- Properties is automatically added.
--



-- plugins stores AppPaths

cpp.Run("Generate", lib, {output=output})

print("Library:")
print(lib)


function install()

end

-- In the future try it this way:
-- lib = project:Library
-- {
-- 	name="lib",
--     sources= {FileSet.Create("src/main/mcpp", ".mcpp$")},
--     dependencies = {}
-- }
