--------------------------------------------------------------------------------
-- Plugins
--------------------------------------------------------------------------------
porg = plugins:Get("Porg")
cpp = plugins:Get("Cpp")
html = plugins:Get("HtmlView")
bjam = plugins:Get("BoostBuild2")


--------------------------------------------------------------------------------
-- Dependencies
--------------------------------------------------------------------------------
boost  = load("Macaroni", "Boost-headers", "1.46.1"):Target("lib")
cppstd = load("Macaroni", "CppStd", "2003"):Target("lib")


--------------------------------------------------------------------------------
-- File Paths
--------------------------------------------------------------------------------
sourcePath = "Source"
outputPath = "target"


--------------------------------------------------------------------------------
-- Project Model Information
--------------------------------------------------------------------------------
project = context:Group("Macaroni.Tests")
                       :Project("Features-BoostBuild-SomeLibB")
                       :Version("1.0.0.0");
lib = project:Library{
    name = "lib",
    headers = pathList{sourcePath, outputPath},
    sources = pathList{sourcePath},
    dependencies = {
        cppstd,
        boost
    }
}
porg:Run("Generate", {target=lib})


--------------------------------------------------------------------------------
-- Actions
--------------------------------------------------------------------------------
generated = false
built = false
installed = false

function clean()
    local dir = Macaroni.IO.Path.New(outputPath)
    dir:ClearDirectoryContents();
end

function generate()
  if generated then return end
  local outputPath = filePath(outputPath)
  cpp:Run("Generate", { projectVersion=project, path=outputPath })
  html:Run("Generate", { target=lib, path=outputPath})
  bjam:Run("Generate", { jamroot=outputPath:NewPath("/jamroot.jam"),
                         projectVersion=project,
                         output=output
  })
  generated = true
end

function build()
  if built then return end
  generate()

  -- threading=single --toolset=msvc-11.0
  --os.execute("bjam " .. properties.bjam_options .. " " .. outputPath)
  os.execute("bjam " .. outputPath)
  built = true
end

function install()
  if installed then return end
  build()
  sinstall(project, filePath("./"))
  installed = true

  print(properties.bjam_options)
end
