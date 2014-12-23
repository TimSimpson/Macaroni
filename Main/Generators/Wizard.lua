
function GetMethod(name)
    if name == "CreateProject" then
        return
        {
            Describe = function(args)
                args.output.WriteLine("Creates a new Project in "
                    .. tostring(args.path) .. ".")
            end,
            Run = function(args)
                createNewProject(args.output, args.path, args.projName)
            end
        }
    end
    return nil;
end

function createNewProject(output, path, projName)
    output:WriteLine("Generating project " .. projName .. ".")
    createProjectLua(output, path, projName)
    createHelloWorldMcpp(output, path)
end

function createProjectLua(output, path, projName)
    local projectLuaPath = path:NewPathForceSlash("project.lua")
    output:WriteLine("Generating new project.lua at "
        .. projectLuaPath.AbsolutePathForceSlash .. ".");
    local projectLua = projectLuaPath:CreateFile();
    projectLua:Write([[
import("Macaroni", "ProjectTemplates", "1")
require "SimpleProject"

SimpleProject{
  group="your-name-here",
  project="]] .. projName .. [[",
  version="DEV",
  libShortName = "]] .. projName .. [[Lib",
  src="src",
  target="target",
  dependencies = {
    load("Macaroni", "Boost-headers", "1.57"):Target("lib"),
    -- Uncomment this out to use Boost Test:
    -- load("Macaroni", "Boost-unit_test_framework", "1.57"):Target("lib"),
    load("Macaroni", "CppStd", "2003"):Target("lib"),
  },
};

-- The code below shows how to override the generate function created by
-- SimpleProject with your own. The same trick will work for the clean,
-- preGenerate, generate, and install functions.
--
-- spBuild = build
--
-- function build()
--    print("Do some custom stuff here before the typical build-")
--    spBuild()
--    print("- then do more after it.")
-- end
    ]])
    output:WriteLine("WTF?!")
end


function createHelloWorldMcpp(output, path)
    path:NewPathForceSlash("src"):CreateDirectory()
    local helloMcpp = path:NewPathForceSlash("src/hello.mcpp"):CreateFile()
    helloMcpp:Write(
[[// This directive tells Macaroni to put everything we create into a H file
// / CPP file pair called "HelloWorld", and to also tell the build file to
// create an executable with that name.
//
// Note that Macaroni will by default put any plain .C, .CC, or .CPP files it
// finds in the source directory into the main library generated for a project.
// To explicitly put them into a different build target requires explicitly
// stating which target owns them in the project.lua file.
//
~unit "HelloWorld" type=exe
{
    // Macaroni knows of the following imports thanks to its dependency on the
    // project Macaroni/CppStd/2003, as stated in project.lua.
    ~import std::cin;
    ~import std::cout;
    ~import std::endl;

    // Macaroni always assumes imports exist, meaning problems don't arise
    // until the code generation phase. This is defined in world.mcpp.
    // Macaroni always uses full logical paths of a type name to bring
    // an item into scope rather than the physical file path.
    ~import MacaroniExample::World;

    ~block "cpp"
    {
        int main(int argc, char ** argv)
        {
            cout << "Hello World!" << endl;
            World world;
            cout << world.Response() << endl;
        }
    }
}]])
    local worldMcpp = path:NewPathForceSlash("src/world.mcpp"):CreateFile()
    worldMcpp:Write([[
// Defines the class "MacaroniExample::World". Macaroni will by default put
// this into an H file / CPP file pair located at MacaroniExample/World.[h|cpp]
// in the generated output directory, which by default is "target".
~import std::string;

~namespace MacaroniExample;

class World
{
    public string Response() const
    {
        return ":)";
    }
};
]])
    local worldTestMcpp = path:NewPathForceSlash("src/worldTest.mcpp")
        :CreateFile()
    worldTestMcpp:Write(
[[// A simple unit test for the World class. Note you could cut and paste the
// contents of this file into world.mcpp and everything would work the same.
~unit "WorldTest" type=test
{
    ~import MacaroniExample::World;

    ~block "cpp"
    {
        int main(int argc, char ** argv)
        {
            World w;
            // Return a good exit code (zero) if the value is as expected.
            return w.Response() == ":)" ? 0 : 1;
        }
    }

    // This is a superior way of writing the above test using the Boost Test
    // framework. To enable it, erase the ~block above and uncomment this
    // section below, and uncomment the part in project.lua that loads Boost
    // Test. Note that with the section commented out, Macaroni won't even
    // generate the code block.
    /*
    ~block "cpp"
    {
        #define BOOST_TEST_MODULE WorldTests
        #include <boost/test/unit_test.hpp>

        BOOST_AUTO_TEST_CASE(receive_happy_response)
        {
            World w;
            BOOST_REQUIRE_EQUAL(":)", w.Response());
        }
    }
    */
}
]])
end
