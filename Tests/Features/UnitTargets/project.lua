description = [[
The goal of this test is to establish granularity within a library by compiling
a simple C++ app to the same degree of specificity as could be achieved by
Boost-Build.

The "manifest" build system was very broad; libraries were defines as blobs of
C++ files which all got combined. The problem was Macaroni never established
how to do this, so it was impossible to write generators to say something such
as "A.cpp is used by B.cpp, and has a single unit test called A-Test.cpp". The
goal with granular UnitTargets it to be able to generate build files and other
goodies that can figure this out in a compatable fashion.

]]


project = context:Group("Macaroni.Tests")
                       :Project("Features-UnitTargets")
                       :Version("1.0.0.0");

cppstd = load("Macaroni", "CppStd", "2003"):Target("lib")
lib = project:Library{
    name = "lib",
    -- Important note: When the library sees a ProjectVersion as a
    -- dependency, it automatically uses a target named "lib".
    -- dependencies = {boost_regex_lib, boost_headers},
    headers = pathList{"src"},
    sources = pathList{"src", ".mcpp$"},

    dependencies = {
        cppstd
    }
}


porg = plugins:Get("Porg")
porg:Run("Generate", {target=lib})

cpp = plugins:Get("Cpp")
local outputPath = filePath("target") --ath.New("target")
cpp:Run("Generate", { target=lib, path=outputPath })
