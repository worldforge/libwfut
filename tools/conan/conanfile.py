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
    options = {"shared": [False, True], "fPIC": [True, False]}
    default_options = {"shared": False, "fPIC": True}
    generators = "cmake_find_package", "cmake_paths"
    requires = ["sigc++/2.10.0@worldforge/stable",
                "zlib/1.2.13",
                "libcurl/7.87.0"]

    scm = {
        "type": "git",
        "url": "https://github.com/worldforge/libwfut.git",
        "revision": "auto"
    }

    def _configure_cmake(self):
        cmake = CMake(self)
        cmake.configure(source_folder=".")
        return cmake

    def build(self):
        cmake = self._configure_cmake()
        cmake.build()

    def package_info(self):
        self.cpp_info.libs = tools.collect_libs(self)

    def package(self):
        cmake = self._configure_cmake()
        cmake.install()
