Files = Macaroni.Model.Project.Files
FileSet = Macaroni.IO.FileSet;

project = context:Group("Macaroni")
                 :Project("CppStd")
                 :Version("2003");

project:Library{ name="lib" }:AddArtifacts("src")

-- TODO: Move this to Dependencies and install it fresh each time.

