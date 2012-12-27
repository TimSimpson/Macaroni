require "os"
require "Macaroni.Model.Library"

upper = getUpperLibrary();
id =
{
    group=upper.Group,
    name="Macaroni.Tests",
    version=upper.Version
}

description="Battery of Tests for Macaroni."

cavatappi("cavatappi.lua");