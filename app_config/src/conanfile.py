from conan import ConanFile

class ConfigRecipe(ConanFile):
    python_requires = "cpp-tools/1.0@pasan/testing"
    python_requires_extend = "cpp-tools.BaseRecipe"

    def init(self):
        base = self.python_requires["cpp-tools"].module.BaseRecipe
        base.name = "app_config"
        base.version = "1.0"
        base.exports_sources = "CMakeLists.txt", "config/*"

    def requirements(self):
        try:
            super().requirements()
        except AttributeError:
            pass
        self.requires("app_utils/1.0@pasan/testing")
