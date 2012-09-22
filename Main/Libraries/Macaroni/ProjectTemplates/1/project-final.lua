Files = Macaroni.Model.Project.Files
FileSet = Macaroni.IO.FileSet;

project = context:Group("Macaroni")
                 :Project("ProjectTemplates")
                 :Version("1");

project:Resource{name = "scripts", type="lua", files = pathList{"src"}}
