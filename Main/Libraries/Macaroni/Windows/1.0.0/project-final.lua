project = context:Group("Macaroni")
                 :Project("Windows")
                 :Version("1.0.0");

lib = project:Library
{
    name="lib",
    sources = pathList{"src"},
}
