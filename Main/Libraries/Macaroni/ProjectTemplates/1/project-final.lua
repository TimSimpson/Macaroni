local Files = require "Macaroni.Model.Project.Files"
local FileSet = require "Macaroni.IO.FileSet"

project = context:Group("Macaroni")
                 :Project("ProjectTemplates")
                 :Version("1");

project:Resource{name = "scripts", type="lua", files = pathList{"src"}}
