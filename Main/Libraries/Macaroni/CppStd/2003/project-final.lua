project = context:Group("Macaroni")
                 :Project("CppStd")
                 :Version("2003");

-- project:Library{
--     name="lib",
--     sources = files{
--         Macaroni = { fileSet("Source") }
--     }
-- }


lib = project:Library
{
    name="lib",
    sources = pathList{"Source"}
}
