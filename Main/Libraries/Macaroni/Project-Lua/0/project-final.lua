Files = Macaroni.Model.Project.Files
FileSet = Macaroni.IO.FileSet;

project = context:Group("Macaroni")
                 :Project("Project-Lua")
                 :Version("0");

project:Resource{name = "scripts", type="lua", files = pathList{"src"}}
--project:LuaScripts{name = "scripts", files=pathList("src")}
-- TODO: Move this to Dependencies and install it fresh each time.

