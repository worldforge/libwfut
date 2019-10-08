from conans import ConanFile, CMake, tools


class LibWFUTConan(ConanFile):
    name = "libwfut"
    version = "0.2.4"
    license = "GPL-2.0+"
    author = "Erik Ogenvik <erik@ogenvik.org>"
    homepage = "https://www.worldforge.org"
    url = "https://github.com/worldforge/libwfut"
    description = "A client side C++ implementation of WFUT (WorldForge Update Tool)."
    topics = ("mmorpg", "worldforge")
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [False, True]}
    default_options = {"shared": False}
    generators = "cmake"
    requires = ["sigc++/2.10.0@bincrafters/stable",
                "zlib/1.2.11",
                "libcurl/7.64.1@bincrafters/stable"]

    scm = {
        "type": "git",
        "url": "https://github.com/worldforge/libwfut.git",
        "revision": "auto"
    }

    def build(self):
        cmake = CMake(self)
        cmake.configure(source_folder=".")
        cmake.build()
        cmake.install()

    def package_info(self):
        self.cpp_info.libs = tools.collect_libs(self)
        self.cpp_info.includedirs = ["include/wfut-0.2"]

    def package(self):
        pass
