from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMake, cmake_layout

class H3Recipe(ConanFile):
    python_requires = "cpp-tools/1.0"
    python_requires_extend = "cpp-tools.baseRecipe"

    def init(self):
        base = self.python_requires["cpp-tools"].module.baseRecipe
        base.name = "app_h3"
        base.version = "1.0"
        base.exports_sources = "CMakeLists.txt", "h3/*"
