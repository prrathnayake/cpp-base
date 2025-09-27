from conan import ConanFile

class AuthRecipe(ConanFile):
    python_requires = "cpp-tools/1.0@pasan/testing"
    python_requires_extend = "cpp-tools.BaseRecipe"

    def init(self):
        base = self.python_requires["cpp-tools"].module.BaseRecipe
        base.name = "app_auth"
        base.version = "1.0"
        base.exports_sources = "CMakeLists.txt", "auth/*"
