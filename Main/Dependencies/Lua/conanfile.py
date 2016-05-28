from conans import ConanFile, CMake
from conans import tools
import os
import os.path
import shutil

class GreetConan(ConanFile):
    name = "Lua"
    version = "5.2.3"
    settings = "os", "compiler", "build_type", "arch"
    lua_dir = "lua-5.2.3"
    exports = ["conanfile.py", "lua-CMakeLists.txt", "Mh/Interface.mh"]

    def source(self):
        archive = "{}.tar.gz".format(self.lua_dir)
        tools.download("https://www.lua.org/ftp/lua-5.2.3.tar.gz", archive)
        tools.unzip(archive, ".")
        os.remove(archive)
        shutil.copy("lua-CMakeLists.txt",
                    os.path.join(self.lua_dir, "CMakeLists.txt"))

    def build(self):
        cmake = CMake(self.settings)
        self.run('cmake %s/%s %s'
                 % (self.conanfile_directory, self.lua_dir, cmake.command_line))
        self.run("cmake --build . %s" % cmake.build_config)

    def package(self):
        self.copy("*.h", dst="include", src=os.path.join(self.lua_dir, "src"))
        self.copy("*.lib", dst="lib", src="lib")
        self.copy("*.a", dst="lib", src="lib")

    def package_info(self):
        self.cpp_info.libs = ["lua"]
