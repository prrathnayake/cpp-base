from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMake, cmake_layout

class rabbitMQRecipe(ConanFile):
    python_requires = "cpp-tools/1.0@pasan/testing"
    python_requires_extend = "cpp-tools.BaseRecipe"

    def init(self):
        base = self.python_requires["cpp-tools"].module.BaseRecipe
        base.name = "app_rabbitmq"
        base.version = "1.0"
        base.exports_sources = "CMakeLists.txt", "rabbitMQ/*"
        base.requires = "app_utils/1.0@pasan/testing"
 