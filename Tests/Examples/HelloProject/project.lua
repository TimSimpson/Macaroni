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

print("Library:")
print(lib)

-- In the future try it this way:
-- lib = project:Library
-- {
-- 	name="lib",
--     sources= {FileSet.Create("src/main/mcpp", ".mcpp$")},
--     dependencies = {}
-- }
