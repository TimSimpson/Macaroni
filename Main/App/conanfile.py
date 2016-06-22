from conans import ConanFile, CMake
from conans import tools
import os
import os.path
import shutil


channel = os.getenv("CONAN_CHANNEL", "testing")
username = os.getenv("CONAN_USERNAME", "demo")


class MacaroniConan(ConanFile):
    name = "Macaroni"
    version = "0.4.0"
    requires = ("Lua/5.2.3@%s/%s" % (username, channel),
                "Boost/1.60.0@lasote/stable")
    settings = "os", "compiler", "build_type", "arch"
    exports = ["PureCpp"]
    generators = "cmake"

    def build(self):
        root = self.conanfile_directory
        cmd =  ('macaroni {root}  '
                            '--messagesPath {messages} '
                            '--libraryRepoPath={libraries} '
                            '--showPaths --printTimes --verbose -g'
                .format(
                    root=root,
                    messages=os.path.join(root, 'Source', 'main', 'resources',
                                          'Messages.txt'),
                    libraries=os.path.join(root, '..', 'Libraries')))

        self.run("cd %s && %s" % (self.conanfile_directory, cmd))
        cmake = CMake(self.settings)
        self.run('cmake %s %s' % (self.conanfile_directory, cmake.command_line))
        self.run("cmake --build . %s" % cmake.build_config)

    def package(self):
        #self.copy("*.h", dst="include", src=os.path.join(self.lua_dir, "src"))
        #self.copy("*.lib", dst="lib", src="lib")
        #self.copy("*.a", dst="lib", src="lib")
        pass

    def package_info(self):
        self.cpp_info.libs = ["LibMacaroni"]
