from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMake, cmake_layout

class kafkaRecipe(ConanFile):
    name = "app_kafka"
    version = "1.0"

    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False]}
    default_options = {"shared": True}

    exports_sources = "CMakeLists.txt", "kafka/*"
    
    def requirements(self):
        self.requires("app_utils/1.0")

    def layout(self):
        cmake_layout(self)

    def generate(self):
        tc = CMakeToolchain(self)
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()
        
    def package_info(self):
        self.cpp_info.libs = ["app_kafka"]
